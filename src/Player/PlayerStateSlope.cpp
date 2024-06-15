#include "Player/PlayerStateSlope.h"
#include "CUSTOM/PlayerAnimator.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/stuff.h"
#include "Player/PlayerActionSlopeSlideControl.h"
#include "Player/PlayerConst.h"
#include "Stuff.h"
#include "Util/PlayerCollisionUtil.h"

namespace {
NERVE_IMPL(PlayerStateSlope, Slide)
NERVE_MAKE(PlayerStateSlope, Slide)
}  // namespace

PlayerStateSlope::PlayerStateSlope(al::LiveActor* player, PlayerConst const* pConst,
                                   PlayerInput const* input, IUsePlayerCollision const* collision,
                                   PlayerAnimator* animator)
    : al::ActorStateBase("坂すべり", player), mConst(pConst), mCollision(collision),
      mAnimator(animator) {
    mActionSlopeSlideControl = new PlayerActionSlopeSlideControl(player, pConst, input, collision);
    initNerve(&Slide, 0);
}
PlayerStateSlope::~PlayerStateSlope() = default;

void PlayerStateSlope::appear() {
    mIsDead = false;
    mActionSlopeSlideControl->setup();
    mTimeInAir = 0;
    mSlopeForceCounter = 0;
    mIsForceSlide = false;
    mIsRunningRumbleLoop = false;

    if(rs::isOnGroundForceSlideCode(mActor, mCollision, mConst)) {
        if(al::isNearZero(mActionSlopeSlideControl->_20, 0.001f))
            al::setVelocityZero(mActor);
        else {
            sead::Vector3f* velocityPtr = al::getVelocityPtr(mActor);
            al::parallelizeVec(velocityPtr, mActionSlopeSlideControl->_20, *velocityPtr);
        }
        mIsForceSlide = true;   
    }

    f32 dot = mActionSlopeSlideControl->_20.dot(al::getVelocity(mActor));
    if (dot < 5.0f)
        al::addVelocityToDirection(mActor, mActionSlopeSlideControl->_20, 5.0f - dot);

    sead::Vector3f* velocityPtr = al::getVelocityPtr(mActor);
    al::limitLength(velocityPtr, *velocityPtr, mConst->getSlopeSlideMaxSpeed());
    al::setNerve(this, &Slide);
}

void PlayerStateSlope::kill() {
    mIsDead = true;
    if(mIsRunningRumbleLoop) {
        alPadRumbleFunction::stopPadRumbleLoop(mActor, "【ループ】ジリジリ（中）", al::getTransPtr(mActor), -1);
        mIsRunningRumbleLoop = false;
    }
}

bool PlayerStateSlope::isEnableCancelSandSink() const {
    return mSlopeForceCounter > 15;
}

f32 PlayerStateSlope::calcSlideSpeed() const {
    return mActionSlopeSlideControl->_38.dot(mActionSlopeSlideControl->_20);
}

void PlayerStateSlope::exeSlide() {
    if(al::isFirstStep(this)) {
        mAnimator->startAnim("Slide");
        mTimeInAir = 0;
        mSlopeForceCounter = 0;
    }

    if(rs::isCollidedGround(mCollision)) {
        if(!mIsRunningRumbleLoop) {
            alPadRumbleFunction::startPadRumbleLoopNo3D(mActor, "【ループ】ジリジリ（中）", al::getTransPtr(mActor), -1);
            mIsRunningRumbleLoop = true;
        }
    } else if(mIsRunningRumbleLoop) {
        alPadRumbleFunction::stopPadRumbleLoop(mActor, "【ループ】ジリジリ（中）", al::getTransPtr(mActor), -1);
        mIsRunningRumbleLoop = false;
    }

    bool isForceSlide = rs::isOnGroundForceSlideCode(mActor, mCollision, mConst);
    if(!mIsForceSlide && isForceSlide) {
        if(al::isNearZero(mActionSlopeSlideControl->_20, 0.001f))
            al::setVelocityZero(mActor);
        else {
            sead::Vector3f* velocityPtr = al::getVelocityPtr(mActor);
            al::parallelizeVec(velocityPtr, mActionSlopeSlideControl->_20, *velocityPtr);
        }
        mIsForceSlide = true;
    }

    mActionSlopeSlideControl->update(
        mConst->getSlopeSlideAccel(),
        isForceSlide ? 0.0f : mConst->getSlopeSlideBrake(),
        0.0f, 0.0f,
        mConst->getSlopeSlideMaxSpeed(),
        isForceSlide ? mConst->getSlopeSlideForceSideAccel() : mConst->getSlopeSlideSideAccel(),
        isForceSlide ? mConst->getSlopeSlideForceSideBrake() : mConst->getSlopeSlideSideBrake(),
        isForceSlide ? mConst->getSlopeSlideForceSideMaxSpeed() : mConst->getSlopeSlideSideMaxSpeed(),
        isForceSlide ? mConst->getSlopeSlideForceTurnDegree() : mConst->getSlopeTurnDegree(),
        mConst->getGravityMove(), 0);
    
    if(rs::isOnGroundSlopeSlideEnd(mActor, mCollision, mConst)) {
        if(rs::isJustLand(mCollision) && mTimeInAir >= 30) {
            kill();
            return;
        }

        mSlopeForceCounter = al::converge(mSlopeForceCounter, mConst->getSlopeForceFrame(), 1);
        if(mActionSlopeSlideControl->_38.length() <= mConst->getSlopeSlideSpeedEnd() && al::isGreaterEqualStep(this, mConst->getSlopeForceFrame())) {
            kill();
            return;
        }
    } else {
        mSlopeForceCounter = 0;
        if(rs::isCollidedGround(mCollision)) {
            mTimeInAir = 0;
        } else {
            mTimeInAir = al::converge(mTimeInAir, 30, 1);
        }
    }
}
