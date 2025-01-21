#include "Player/HackerStateWingFly.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/Hack.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerCollisionUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_HOST_TYPE_IMPL(HackerStateWingFly, FlyRiseToHighest);
NERVE_HOST_TYPE_IMPL(HackerStateWingFly, FlyRiseToTop);
NERVE_HOST_TYPE_IMPL(HackerStateWingFly, FlyTop);
NERVE_HOST_TYPE_IMPL(HackerStateWingFly, Fall);
NERVE_HOST_TYPE_IMPL(HackerStateWingFly, FallFly);
NERVE_HOST_TYPE_IMPL(HackerStateWingFly, Trample);
NERVE_HOST_TYPE_IMPL(HackerStateWingFly, UpperPunch);

NERVES_MAKE_STRUCT(HostType, FlyRiseToHighest, Fall, Trample, UpperPunch, FlyRiseToTop, FlyTop,
                   FallFly);
}  // namespace

bool isTriggerHacker(IUsePlayerHack** hacker) {
    return rs::isTriggerHackAnyButton(*hacker) || rs::isTriggerHackSwing(*hacker);
}

HackerStateWingFly::HackerStateWingFly(al::LiveActor* actor, IUsePlayerHack** hacker,
                                       IUsePlayerCollision* collision)
    : al::ActorStateBase("はばたき飛行", actor), mHacker(hacker), mCollision(collision) {
    mTrans.y = sead::Mathf::minNumber() / 2.0f;
    initNerve(&NrvHostType.FlyRiseToHighest, 0);
}

void HackerStateWingFly::appear() {
    setDead(false);
    mFallTimeDelay = 0;

    if (_90) {
        _90 = false;
        al::setNerve(this, &NrvHostType.Fall);
        return;
    }

    goFlyRise();
}

void HackerStateWingFly::goFlyRise() {
    al::Nerve* nerve;

    if (al::getTrans(mActor).y < mTrans.y)
        nerve = &NrvHostType.FlyRiseToHighest;
    else
        nerve = &NrvHostType.FlyRiseToTop;

    al::setNerve(this, nerve);
}

void HackerStateWingFly::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    al::Nerve* nerve;
    if (mActionTrample == nullptr || !rs::trySendMsgPlayerReflectOrTrample(mActor, self, other)) {
        if (!canUpperPunch(self, other) ||
            (!al::sendMsgPlayerUpperPunch(other, self) && !rs::sendMsgHackUpperPunch(other, self) &&
             !rs::sendMsgHackObjUpperPunch(other, self)))
            return;

        al::setVelocityZeroV(mActor);
        nerve = &NrvHostType.UpperPunch;
    } else {
        nerve = &NrvHostType.Trample;
    }
    al::setNerve(this, nerve);
}

bool HackerStateWingFly::canUpperPunch(al::HitSensor* self, al::HitSensor* other) const {
    if (mActionUpperPunch == nullptr)
        return false;
    if (al::isNerve(this, &NrvHostType.Fall))
        return false;
    if (al::isNerve(this, &NrvHostType.FallFly))
        return false;
    if (!al::isSensorName(self, "Head"))
        return false;

    sead::Vector3f sensors;
    sead::Vector3f gravity = -al::getGravity(mActor);
    al::calcVecBetweenSensors(&sensors, self, other);
    if (sensors.x * gravity.x + sensors.y * gravity.y + sensors.z * gravity.z < 0.0f)
        return false;

    al::parallelizeVec(&sensors, gravity, sensors);
    if (al::getSensorRadius(other) < sensors.length()) {
        if (al::isNerve(this, &NrvHostType.UpperPunch) && isOnGround())
            return false;

        return true;
    }

    return false;
}

void HackerStateWingFly::updateFlyLimit() {
    if (mTrans.y < al::getTrans(mActor).y)
        mTrans.set(al::getTrans(mActor));
}

bool HackerStateWingFly::judgeStart() {
    if (!isTriggerHacker(mHacker))
        return false;

    al::LiveActor* actor = mActor;
    if (rs::isTriggerHackSwing(*mHacker)) {
        al::setActionFrameRate(actor, mSwingFramerate);
        mAccel = mSwingAccel;
        mVelocityY = mSwingVelocityY;
        return true;
    }

    al::setActionFrameRate(actor, mDefaultFramerate);
    mAccel = mDefaultAccel;
    mVelocityY = mDefaultVelocityY;
    return true;
}

void HackerStateWingFly::updateMove() {
    al::LiveActor* actor = mActor;
    sead::Vector3f dir = sead::Vector3f(0.0f, 0.0f, 0.0f);

    rs::addHackActorAccelStick(actor, *mHacker, &dir, mAccel, sead::Vector3f::ey);
    al::turnToDirection(actor, dir, mTurnAngle);

    if (mCollision != nullptr) {
        rs::reboundVelocityFromCollision(actor, mCollision, 0.0f, 0.0f, 1.0f);
        return;
    }

    if (al::isCollidedCeiling(actor)) {
        const sead::Vector3f& ceilingNormal = al::getCollidedCeilingNormal(actor);
        f32 dotProduct = ceilingNormal.dot(al::getVelocity(actor));
        if (dotProduct < 0.0f)
            al::getVelocityPtr(actor)->add(-ceilingNormal * dotProduct * 1.1f);
    }

    if (al::isCollidedWall(actor)) {
        const sead::Vector3f& wallNormal = al::getCollidedWallNormal(actor);
        f32 dotProduct = wallNormal.dot(al::getVelocity(actor));
        if (dotProduct < 0.0f)
            al::getVelocityPtr(actor)->add(-wallNormal * dotProduct * 1.1f);
    }
}

bool HackerStateWingFly::isOnGround() const {
    al::LiveActor* actor = mActor;
    if (mCollision != nullptr)
        return rs::isOnGround(actor, mCollision);

    return al::isOnGround(actor, 0);
}

bool HackerStateWingFly::tryUpperPunchToCollision() {
    al::LiveActor* actor = mActor;
    bool isCollidedCeiling =
        mCollision == nullptr ? al::isCollidedCeiling(actor) : rs::isCollidedCeiling(mCollision);

    if (!isCollidedCeiling)
        return false;

    al::HitSensor* other = mCollision == nullptr ? al::tryGetCollidedCeilingSensor(actor) :
                                                   rs::tryGetCollidedCeilingSensor(mCollision);
    al::HitSensor* self = al::getHitSensor(actor, "Head");

    if (!al::sendMsgPlayerUpperPunch(other, self))
        return false;

    al::setVelocityZeroV(actor);
    al::setNerve(this, &NrvHostType.UpperPunch);
    return true;
}

void HackerStateWingFly::updateFlyAction() {
    al::LiveActor* actor = mActor;
    if (al::isFirstStep(this)) {
        if (!al::isActionPlaying(actor, "FallFly") ||
            (al::isActionPlaying(actor, "FallFly") && al::isActionEnd(actor)))
            al::tryStartActionIfNotPlaying(actor, mActionFly);

        mFallTimeDelay = 0;
    }

    if (al::isActionPlaying(actor, "FallFly") && al::isActionEnd(actor))
        al::tryStartActionIfNotPlaying(actor, mActionFly);

    if (!isTriggerHacker(mHacker))
        return;

    if (rs::isTriggerHackSwing(*mHacker)) {
        al::setActionFrameRate(actor, mSwingFramerate);
        mAccel = mSwingAccel;
        mVelocityY = mSwingVelocityY;
    } else {
        al::setActionFrameRate(actor, mDefaultFramerate);
        mAccel = mDefaultAccel;
        mVelocityY = mDefaultVelocityY;
    }
    mFallTimeDelay = 0;
}

void HackerStateWingFly::exeFlyRiseToHighest() {
    al::LiveActor* actor = mActor;
    updateFlyAction();
    if (tryUpperPunchToCollision())
        return;

    if (al::getTrans(actor).y > mTrans.y + mYLerpValue) {
        al::setNerve(this, &NrvHostType.FlyTop);
        return;
    }

    if (al::getTrans(actor).y >= mTrans.y) {
        al::setNerve(this, &NrvHostType.FlyRiseToTop);
        return;
    }

    al::setVelocityY(actor, mVelocityY);
    al::addVelocityToGravity(actor, 4.0f);
    al::scaleVelocityHV(actor, 0.95f, 0.7f);
    if (0x15 <= mFallTimeDelay) {
        al::setNerve(this, &NrvHostType.Fall);
        return;
    }

    mFallTimeDelay++;
    updateMove();
    return;
}

void HackerStateWingFly::exeFlyRiseToTop() {
    al::LiveActor* actor = mActor;
    updateFlyAction();
    if (tryUpperPunchToCollision())
        return;

    f32 endVelY;
    if (al::getTrans(actor).y < mTrans.y) {
        endVelY = mVelocityY;
    } else {
        f32 lerpTime = al::calcNerveSquareOutRate(this, 30);
        f32 posY = mTrans.y + al::lerpValue(0.0f, mYLerpValue, lerpTime);
        endVelY = posY - getTrans(actor).y;
    }

    if (0.0f < endVelY) {
        f32 startVelY = al::getVelocity(actor).y;
        endVelY = al::lerpValue(startVelY, endVelY, mLerpTime);
        al::setVelocityY(actor, endVelY);
    }

    al::scaleVelocityHV(actor, 0.95f, 0.7f);
    if (al::getTrans(actor).y >= mTrans.y + mYLerpValue) {
        al::setNerve(this, &NrvHostType.FlyTop);
        return;
    }

    if (mFallTimeDelay >= 0x15) {
        al::setNerve(this, &NrvHostType.Fall);
        return;
    }

    mFallTimeDelay++;
    updateMove();
}

void HackerStateWingFly::exeFlyTop() {
    al::LiveActor* actor = mActor;
    updateFlyAction();

    sead::Vector3f trans = al::getTrans(actor);
    f32 newYLerpValue = mTrans.y + mYLerpValue;
    if (trans.y > newYLerpValue) {
        al::getTransPtr(actor)->y = al::lerpValue(al::getTrans(actor).y, newYLerpValue, 0.01f);
        al::setVelocityZeroY(actor);
    }

    if (tryUpperPunchToCollision())
        return;

    al::scaleVelocityHV(mActor, 0.95f, 0.7f);
    if (0x15 <= mFallTimeDelay) {
        al::setNerve(this, &NrvHostType.Fall);
        return;
    }

    mFallTimeDelay++;
    updateMove();
}

void HackerStateWingFly::exeFall() {
    if (al::isFirstStep(this)) {
        if (!al::isActionPlaying(mActor, mActionFly)) {
            al::startAction(mActor, mActionFall);
            al::setActionFrameRate(mActor, 1.0f);
            mAccel = mDefaultAccel;
            mVelocityY = mDefaultVelocityY;
        }
        mActionFrame = 0;
    }

    if (al::isActionPlaying(mActor, mActionFly) && al::getActionFrame(mActor) < mActionFrame) {
        al::startAction(mActor, mActionFall);
        al::setActionFrameRate(mActor, 1.0f);
        mAccel = mDefaultAccel;
        mVelocityY = mDefaultVelocityY;
    } else {
        mActionFrame = al::getActionFrame(mActor);
    }

    al::addVelocityToGravity(mActor, mGravityVelocity);
    al::scaleVelocityHV(mActor, 0.95f, 0.94f);
    if (!isTriggerHacker(mHacker)) {
        if (isOnGround()) {
            kill();
            return;
        }
        updateMove();
        return;
    }

    al::setVelocityZeroV(mActor);
    al::LiveActor* actor = mActor;
    if (rs::isTriggerHackSwing(*mHacker)) {
        al::setActionFrameRate(actor, mSwingFramerate);
        mAccel = mSwingAccel;
        mVelocityY = mSwingVelocityY;
    } else {
        al::setActionFrameRate(actor, mDefaultFramerate);
        mAccel = mDefaultAccel;
        mVelocityY = mDefaultVelocityY;
    }
    al::setNerve(this, &NrvHostType.FallFly);
}

void HackerStateWingFly::exeFallFly() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "FallFly");

    bool isLessStep = al::isLessStep(this, 20);
    al::LiveActor* actor = mActor;
    if (!isLessStep) {
        al::addVelocityToGravity(actor, mGravityVelocity);
        actor = mActor;
    }

    al::scaleVelocityHV(actor, 0.95f, 0.94f);
    if (isTriggerHacker(mHacker)) {
        al::Nerve* nerve;
        if (al::getTrans(mActor).y < mTrans.y)
            nerve = &NrvHostType.FlyRiseToHighest;
        else
            nerve = &NrvHostType.FlyRiseToTop;

        al::setNerve(this, nerve);
        return;
    }

    updateMove();
    if (isOnGround()) {
        kill();
        return;
    }

    if (al::isActionEnd(mActor))
        al::setNerve(this, &NrvHostType.Fall);
}

void HackerStateWingFly::exeTrample() {
    al::LiveActor* actor = mActor;
    if (al::isFirstStep(this)) {
        al::startAction(actor, mActionTrample);
        al::setVelocityZeroV(actor);
        al::addVelocityJump(actor, 20.0f);
    }

    if (al::isActionEnd(actor)) {
        al::setNerve(this, &NrvHostType.Fall);
        return;
    }

    if (!isTriggerHacker(mHacker)) {
        if (isOnGround()) {
            kill();
            return;
        }
        al::addVelocityToGravity(actor, mGravityVelocity);
        al::scaleVelocityHV(actor, 0.95f, 0.94f);
        updateMove();
        return;
    }

    if (rs::isTriggerHackSwing(*mHacker)) {
        al::setActionFrameRate(actor, mSwingFramerate);
        mAccel = mSwingAccel;
        mVelocityY = mSwingVelocityY;
    } else {
        al::setActionFrameRate(actor, mDefaultFramerate);
        mAccel = mDefaultAccel;
        mVelocityY = mDefaultVelocityY;
    }

    al::Nerve* nerve;
    if (al::getTrans(mActor).y < mTrans.y)
        nerve = &NrvHostType.FlyRiseToHighest;
    else
        nerve = &NrvHostType.FlyRiseToTop;

    al::setNerve(this, nerve);
}

void HackerStateWingFly::exeUpperPunch() {
    al::LiveActor* actor = mActor;
    if (al::isFirstStep(this))
        al::startAction(actor, mActionUpperPunch);

    if (al::isActionEnd(actor)) {
        al::setNerve(this, &NrvHostType.Fall);
        return;
    }

    if (!al::isGreaterStep(this, 1) || !isTriggerHacker(mHacker)) {
        al::addVelocityToGravity(actor, mGravityVelocity);
        al::scaleVelocityHV(actor, 0.95f, 0.94f);
        updateMove();
        return;
    }

    if (rs::isTriggerHackSwing(*mHacker)) {
        al::setActionFrameRate(actor, mSwingFramerate);
        mAccel = mSwingAccel;
        mVelocityY = mSwingVelocityY;
    } else {
        al::setActionFrameRate(actor, mDefaultFramerate);
        mAccel = mDefaultAccel;
        mVelocityY = mDefaultVelocityY;
    }

    al::Nerve* nerve;
    if (al::getTrans(mActor).y < mTrans.y)
        nerve = &NrvHostType.FlyRiseToHighest;
    else
        nerve = &NrvHostType.FlyRiseToTop;

    al::setNerve(this, nerve);
}
