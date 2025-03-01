#include "Player/PlayerStateHipDrop.h"

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Player/PlayerAnimator.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerTrigger.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerCollisionUtil.h"

namespace {
NERVE_IMPL(PlayerStateHipDrop, Land);
NERVE_IMPL(PlayerStateHipDrop, Start);
NERVE_IMPL(PlayerStateHipDrop, Fall);

NERVES_MAKE_NOSTRUCT(PlayerStateHipDrop, Land, Start, Fall);
}  // namespace

PlayerStateHipDrop::PlayerStateHipDrop(al::LiveActor* player, const PlayerConst* pConst,
                                       const PlayerInput* input,
                                       const IUsePlayerCollision* collider,
                                       PlayerAnimator* animator, PlayerTrigger* trigger)
    : al::ActorStateBase("ヒップドロップ", player), mConst(pConst), mInput(input),
      mCollision(collider), mAnimator(animator), mTrigger(trigger) {
    initNerve(&Start, 0);
}

void PlayerStateHipDrop::appear() {
    setDead(false);
    if (mAnimator->isSubAnimPlaying())
        mAnimator->endSubAnim();
    mIsLandGround = false;
    mLandPos = {0.0f, 0.0f, 0.0f};
    al::setNerve(this, &Start);
}

bool PlayerStateHipDrop::attackHipDropKnockDown(al::HitSensor* sender, al::HitSensor* receiver) {
    if (isDead())
        return false;
    if (!al::isNerve(this, &Land) || !al::isFirstStep(this))
        return false;
    return al::sendMsgPlayerHipDropKnockDown(receiver, sender);
}

bool PlayerStateHipDrop::isEnableHeadSliding() const {
    if (isDead())
        return false;
    return al::isNerve(this, &Start) || al::isNerve(this, &Fall);
}

bool PlayerStateHipDrop::isEnableHipDropAttack() const {
    if (isDead())
        return false;
    return al::isNerve(this, &Fall) && mHipDropMsgIntervalCounter == 0;
}

bool PlayerStateHipDrop::isEnableLandCancel() const {
    if (isDead())
        return false;
    if (!al::isNerve(this, &Land))
        return false;
    s32 nerveStep = al::getNerveStep(this);
    return nerveStep >= mConst->getJumpHipDropPermitBeginFrame() &&
           nerveStep <= mConst->getJumpHipDropPermitEndFrame();
}

bool PlayerStateHipDrop::isEnableMove() const {
    if (isDead())
        return false;
    return al::isNerve(this, &Land) &&
           al::isGreaterEqualStep(this, mConst->getHipDropLandCancelFrame());
}

bool PlayerStateHipDrop::isEnableInWater() const {
    if (isDead())
        return false;
    if (al::isNerve(this, &Start))
        return false;
    if (al::isNerve(this, &Fall))
        return al::isGreaterEqualStep(this, 1);
    return true;
}

bool PlayerStateHipDrop::isEnableIK() const {
    if (isDead())
        return false;
    return al::isNerve(this, &Land);
}

bool PlayerStateHipDrop::isLandTrigger() const {
    if (mTrigger->isOn(PlayerTrigger::ECollisionTrigger_val1))
        return true;
    return al::isNerve(this, &Land) && al::isFirstStep(this);
}

void PlayerStateHipDrop::exeStart() {
    if (al::isFirstStep(this)) {
        mAnimator->startAnim("HipDropStart");
        al::setVelocityZero(mActor);
        sead::Vector3f negGravity = -al::getGravity(mActor);
        sead::Vector3f up = {0.0f, 0.0f, 0.0f};
        al::calcUpDir(&up, mActor);

        if (up.dot(negGravity) <=
            sead::Mathf::cos(sead::Mathf::deg2rad(mConst->getCollisionResetLimit())))
            mTrigger->set(PlayerTrigger::EActionTrigger_val3);

        sead::Quatf quat;
        al::calcQuat(&quat, mActor);
        al::turnQuatYDirRadian(&quat, quat, negGravity, sead::Mathf::deg2rad(180.0f));
        al::updatePoseQuat(mActor, quat);
    }

    if (mAnimator->isAnimEnd())
        al::setNerve(this, &Fall);
}

void PlayerStateHipDrop::exeFall() {
    if (al::isFirstStep(this)) {
        mAnimator->startAnim("HipDrop");
        al::addVelocityToGravityLimit(mActor, mConst->getHipDropSpeed(),
                                      mConst->getHipDropSpeedMax());
    }

    al::addVelocityToGravityLimit(mActor, mConst->getHipDropGravity(),
                                  mConst->getHipDropSpeedMax());
    if (mTrigger->isOn(PlayerTrigger::ECollisionTrigger_val1) && mInput->isHoldHipDrop()) {
        mHipDropMsgIntervalCounter = mConst->getHipDropMsgInterval();
        mAnimator->startAnim("HipDropReaction");
        mIsLandGround = false;
        if (rs::isCollidedGround(mCollision)) {
            mIsLandGround = true;
            mLandPos = rs::getCollidedGroundPos(mCollision);
        }
    }

    if (mHipDropMsgIntervalCounter > 0) {
        if (rs::convergeOnGroundCount(&mHipDropMsgIntervalCounter, mActor, mCollision, 0, 1))
            return;
        mHipDropMsgIntervalCounter = 0;
    }

    if (rs::isOnGroundRunAngle(mActor, mCollision, mConst))
        al::setNerve(this, &Land);
}

void PlayerStateHipDrop::exeLand() {
    if (al::isFirstStep(this)) {
        mAnimator->startAnim("HipDropLand");

        al::LiveActor* actor = mActor;
        bool isLandingWeak = false;
        if (mIsLandGround) {
            const IUsePlayerCollision* collision = mCollision;
            if (rs::isCollidedGround(collision)) {
                const sead::Vector3f& gravity = al::getGravity(actor);
                isLandingWeak =
                    (rs::getCollidedGroundPos(collision) - mLandPos).dot(gravity) <= 10.0f;
            }
        }
        rs::startHitReactionHipDropLand(actor, isLandingWeak);
    }

    if (rs::isOnGroundRunAngle(mActor, mCollision, mConst)) {
        rs::waitGround(mActor, mCollision, mConst->getGravityMove(), mConst->getFallSpeedMax(),
                       mConst->getSlerpQuatRateWait(), mConst->getWaitPoseDegreeMax());
    } else {
        kill();
    }

    if (mAnimator->isAnimEnd())
        kill();
}
