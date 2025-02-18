#include "Player/PlayerStateSlope.h"

#include "Library/Controller/PadRumbleFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Player/PlayerActionSlopeSlideControl.h"
#include "Player/PlayerAnimator.h"
#include "Player/PlayerConst.h"
#include "Util/PlayerCollisionUtil.h"

namespace {
NERVE_IMPL(PlayerStateSlope, Slide)
NERVES_MAKE_NOSTRUCT(PlayerStateSlope, Slide)
}  // namespace

PlayerStateSlope::PlayerStateSlope(al::LiveActor* player, const PlayerConst* pConst,
                                   const PlayerInput* input, const IUsePlayerCollision* collision,
                                   PlayerAnimator* animator)
    : al::ActorStateBase("坂すべり", player), mConst(pConst), mCollision(collision),
      mAnimator(animator) {
    mActionSlopeSlideControl = new PlayerActionSlopeSlideControl(player, pConst, input, collision);
    initNerve(&Slide, 0);
}

void PlayerStateSlope::appear() {
    setDead(false);
    mActionSlopeSlideControl->setup();

    mTimeInAir = 0;
    mSlopeForceCounter = 0;
    mIsForceSlide = false;
    mIsRunningRumbleLoop = false;

    if (rs::isOnGroundForceSlideCode(mActor, mCollision, mConst)) {
        sead::Vector3f direction = mActionSlopeSlideControl->getDirSlide();
        if (al::isNearZero(direction, 0.001f))
            al::setVelocityZero(mActor);
        else {
            sead::Vector3f* velocityPtr = al::getVelocityPtr(mActor);
            al::parallelizeVec(velocityPtr, direction, *velocityPtr);
        }
        mIsForceSlide = true;
    }

    const sead::Vector3f& direction = mActionSlopeSlideControl->getDirSlide();
    const sead::Vector3f& velocity = al::getVelocity(mActor);
    f32 directedVel = direction.dot(velocity);
    if (directedVel < 5.0f)
        al::addVelocityToDirection(mActor, direction, 5.0f - directedVel);

    sead::Vector3f* velocityPtr = al::getVelocityPtr(mActor);
    al::limitLength(velocityPtr, *velocityPtr, mConst->getSlopeSlideMaxSpeed());
    al::setNerve(this, &Slide);
}

void PlayerStateSlope::kill() {
    setDead(true);

    if (mIsRunningRumbleLoop) {
        alPadRumbleFunction::stopPadRumbleLoop(mActor, "【ループ】ジリジリ（中）",
                                               al::getTransPtr(mActor), -1);
        mIsRunningRumbleLoop = false;
    }
}

bool PlayerStateSlope::isEnableCancelSandSink() const {
    return mSlopeForceCounter > 15;
}

f32 PlayerStateSlope::calcSlideSpeed() const {
    return mActionSlopeSlideControl->getHorizontalVelocity().dot(
        mActionSlopeSlideControl->getDirSlide());
}

void PlayerStateSlope::exeSlide() {
    if (al::isFirstStep(this)) {
        mAnimator->startAnim("Slide");
        mTimeInAir = 0;
        mSlopeForceCounter = 0;
    }

    bool isCollidedGround = rs::isCollidedGround(mCollision);
    al::LiveActor* actor = mActor;
    if (isCollidedGround) {
        if (!mIsRunningRumbleLoop) {
            alPadRumbleFunction::startPadRumbleLoopNo3D(actor, "【ループ】ジリジリ（中）",
                                                        al::getTransPtr(actor), -1);
            mIsRunningRumbleLoop = true;
        }
    } else if (mIsRunningRumbleLoop) {
        alPadRumbleFunction::stopPadRumbleLoop(actor, "【ループ】ジリジリ（中）",
                                               al::getTransPtr(actor), -1);
        mIsRunningRumbleLoop = false;
    }

    bool isForceSlide = rs::isOnGroundForceSlideCode(mActor, mCollision, mConst);
    if (!mIsForceSlide && isForceSlide) {
        sead::Vector3f direction = mActionSlopeSlideControl->getDirSlide();
        if (al::isNearZero(direction, 0.001f))
            al::setVelocityZero(mActor);
        else {
            sead::Vector3f* velocityPtr = al::getVelocityPtr(mActor);
            al::parallelizeVec(velocityPtr, direction, *velocityPtr);
        }
        mIsForceSlide = true;
    }

    mActionSlopeSlideControl->update(
        mConst->getSlopeSlideAccel(), isForceSlide ? 0.0f : mConst->getSlopeSlideBrake(), 0.0f,
        0.0f, mConst->getSlopeSlideMaxSpeed(),
        isForceSlide ? mConst->getSlopeSlideForceSideAccel() : mConst->getSlopeSlideSideAccel(),
        isForceSlide ? mConst->getSlopeSlideForceSideBrake() : mConst->getSlopeSlideSideBrake(),
        isForceSlide ? mConst->getSlopeSlideForceSideMaxSpeed() :
                       mConst->getSlopeSlideSideMaxSpeed(),
        mIsForceSlide ? mConst->getSlopeSlideForceTurnDegree() : mConst->getSlopeTurnDegree(),
        mConst->getGravityMove(), 0);

    if (rs::isOnGroundSlopeSlideEnd(mActor, mCollision, mConst)) {
        if (rs::isJustLand(mCollision) && mTimeInAir >= 30) {
            kill();
            return;
        }

        s32 forceFrame = mConst->getSlopeForceFrame();
        mSlopeForceCounter = al::converge(mSlopeForceCounter, forceFrame, 1);
        bool fastSlide = mActionSlopeSlideControl->getHorizontalVelocity().length() >
                         mConst->getSlopeSlideSpeedEnd();
        if (!fastSlide && al::isGreaterEqualStep(this, mConst->getSlopeForceFrame())) {
            kill();
            return;
        }
    } else {
        mSlopeForceCounter = 0;
        if (rs::isCollidedGround(mCollision))
            mTimeInAir = 0;
        else
            mTimeInAir = al::converge(mTimeInAir, 30, 1);
    }
}
