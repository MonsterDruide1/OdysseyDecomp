#include "Player/HackerStateWingFly.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
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

inline bool isTriggerHacker(IUsePlayerHack** hacker) {
    return rs::isTriggerHackAnyButton(*hacker) || rs::isTriggerHackSwing(*hacker);
}

HackerStateWingFly::HackerStateWingFly(al::LiveActor* actor, IUsePlayerHack** hacker,
                                       IUsePlayerCollision* collision)
    : al::ActorStateBase("はばたき飛行", actor), mHacker(hacker), mCollision(collision) {
    mFlyLimit.y = sead::Mathf::minNumber() / 2.0f;
    initNerve(&NrvHostType.FlyRiseToHighest, 0);
}

void HackerStateWingFly::appear() {
    setDead(false);
    mFallTimeDelay = 0;

    if (mIsJudgeFall) {
        mIsJudgeFall = false;
        al::setNerve(this, &NrvHostType.Fall);
        return;
    }

    goFlyRise();
}

void HackerStateWingFly::goFlyRise() {
    al::Nerve* nerve;

    if (al::getTrans(mActor).y < mFlyLimit.y)
        nerve = &NrvHostType.FlyRiseToHighest;
    else
        nerve = &NrvHostType.FlyRiseToTop;

    al::setNerve(this, nerve);
}

void HackerStateWingFly::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (mParam.actionTrample != nullptr &&
        rs::trySendMsgPlayerReflectOrTrample(mActor, self, other)) {
        al::setNerve(this, &NrvHostType.Trample);
        return;
    }

    if (canUpperPunch(self, other) &&
        (al::sendMsgPlayerUpperPunch(other, self) || rs::sendMsgHackUpperPunch(other, self) ||
         rs::sendMsgHackObjUpperPunch(other, self))) {
        al::setVelocityZeroV(mActor);
        al::setNerve(this, &NrvHostType.UpperPunch);
    }
}

bool HackerStateWingFly::canUpperPunch(al::HitSensor* self, al::HitSensor* other) const {
    if (mParam.actionUpperPunch == nullptr)
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
    if (sensors.dot(gravity) < 0.0f)
        return false;

    al::parallelizeVec(&sensors, gravity, sensors);
    if (!(al::getSensorRadius(other) < sensors.length()))
        return false;

    if (al::isNerve(this, &NrvHostType.UpperPunch) && isOnGround())
        return false;

    return true;
}

void HackerStateWingFly::updateFlyLimit() {
    if (mFlyLimit.y < al::getTrans(mActor).y)
        mFlyLimit.set(al::getTrans(mActor));
}

bool HackerStateWingFly::judgeStart() {
    if (!isTriggerHacker(mHacker))
        return false;

    al::LiveActor* actor = mActor;
    if (rs::isTriggerHackSwing(*mHacker)) {
        al::setActionFrameRate(actor, mParam.swingFramerate);
        mAccel = mParam.swingAccel;
        mVelocityY = mParam.swingVelocityY;
        return true;
    }

    al::setActionFrameRate(actor, mParam.defaultFramerate);
    mAccel = mParam.defaultAccel;
    mVelocityY = mParam.defaultVelocityY;
    return true;
}

void HackerStateWingFly::updateMove() {
    al::LiveActor* actor = mActor;
    sead::Vector3f dir = sead::Vector3f(0.0f, 0.0f, 0.0f);

    rs::addHackActorAccelStick(actor, *mHacker, &dir, mAccel, sead::Vector3f::ey);
    al::turnToDirection(actor, dir, mParam.turnAngle);

    if (mCollision != nullptr) {
        rs::reboundVelocityFromCollision(actor, mCollision, 0.0f, 0.0f, 1.0f);
        return;
    }

    if (al::isCollidedCeiling(actor)) {
        const sead::Vector3f& ceilingNormal = al::getCollidedCeilingNormal(actor);
        f32 speedTowardsCeiling = ceilingNormal.dot(al::getVelocity(actor));
        if (speedTowardsCeiling < 0.0f)
            *al::getVelocityPtr(actor) -= ceilingNormal * speedTowardsCeiling * 1.1f;
    }

    if (al::isCollidedWall(actor)) {
        const sead::Vector3f& wallNormal = al::getCollidedWallNormal(actor);
        f32 speedTowardsWall = wallNormal.dot(al::getVelocity(actor));
        if (speedTowardsWall < 0.0f)
            *al::getVelocityPtr(actor) -= wallNormal * speedTowardsWall * 1.1f;
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
            al::tryStartActionIfNotPlaying(actor, mParam.actionFly);

        mFallTimeDelay = 0;
    }

    if (al::isActionPlaying(actor, "FallFly") && al::isActionEnd(actor))
        al::tryStartActionIfNotPlaying(actor, mParam.actionFly);

    if (!isTriggerHacker(mHacker))
        return;

    if (rs::isTriggerHackSwing(*mHacker)) {
        al::setActionFrameRate(actor, mParam.swingFramerate);
        mAccel = mParam.swingAccel;
        mVelocityY = mParam.swingVelocityY;
    } else {
        al::setActionFrameRate(actor, mParam.defaultFramerate);
        mAccel = mParam.defaultAccel;
        mVelocityY = mParam.defaultVelocityY;
    }
    mFallTimeDelay = 0;
}

void HackerStateWingFly::exeFlyRiseToHighest() {
    al::LiveActor* actor = mActor;
    updateFlyAction();
    if (tryUpperPunchToCollision())
        return;

    if (al::getTrans(actor).y > mFlyLimit.y + mParam.yOvershootMax) {
        al::setNerve(this, &NrvHostType.FlyTop);
        return;
    }

    if (al::getTrans(actor).y >= mFlyLimit.y) {
        al::setNerve(this, &NrvHostType.FlyRiseToTop);
        return;
    }

    al::setVelocityY(actor, mVelocityY);
    al::addVelocityToGravity(actor, 4.0f);
    al::scaleVelocityHV(actor, 0.95f, 0.7f);
    if (mFallTimeDelay >= 21) {
        al::setNerve(this, &NrvHostType.Fall);
        return;
    }

    mFallTimeDelay++;
    updateMove();
}

void HackerStateWingFly::exeFlyRiseToTop() {
    al::LiveActor* actor = mActor;
    updateFlyAction();
    if (tryUpperPunchToCollision())
        return;

    f32 endVelY;
    if (al::getTrans(actor).y < mFlyLimit.y) {
        endVelY = mVelocityY;
    } else {
        f32 flyLerpTime = al::calcNerveSquareOutRate(this, 30);
        f32 posY = mFlyLimit.y + al::lerpValue(0.0f, mParam.yOvershootMax, flyLerpTime);
        endVelY = posY - getTrans(actor).y;
    }

    if (0.0f < endVelY) {
        f32 startVelY = al::getVelocity(actor).y;
        endVelY = al::lerpValue(startVelY, endVelY, mParam.flyStartLerpTime);
        al::setVelocityY(actor, endVelY);
    }

    al::scaleVelocityHV(actor, 0.95f, 0.7f);
    if (al::getTrans(actor).y >= mFlyLimit.y + mParam.yOvershootMax) {
        al::setNerve(this, &NrvHostType.FlyTop);
        return;
    }

    if (mFallTimeDelay >= 21) {
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
    f32 maxFlyY = mFlyLimit.y + mParam.yOvershootMax;
    if (trans.y > maxFlyY) {
        al::getTransPtr(actor)->y = al::lerpValue(al::getTrans(actor).y, maxFlyY, 0.01f);
        al::setVelocityZeroY(actor);
    }

    if (tryUpperPunchToCollision())
        return;

    al::scaleVelocityHV(mActor, 0.95f, 0.7f);
    if (mFallTimeDelay >= 21) {
        al::setNerve(this, &NrvHostType.Fall);
        return;
    }

    mFallTimeDelay++;
    updateMove();
}

void HackerStateWingFly::exeFall() {
    if (al::isFirstStep(this)) {
        if (!al::isActionPlaying(mActor, mParam.actionFly)) {
            al::startAction(mActor, mParam.actionFall);
            al::setActionFrameRate(mActor, 1.0f);
            mAccel = mParam.defaultAccel;
            mVelocityY = mParam.defaultVelocityY;
        }
        mFallFrame = 0;
    }

    if (al::isActionPlaying(mActor, mParam.actionFly) && al::getActionFrame(mActor) < mFallFrame) {
        al::startAction(mActor, mParam.actionFall);
        al::setActionFrameRate(mActor, 1.0f);
        mAccel = mParam.defaultAccel;
        mVelocityY = mParam.defaultVelocityY;
    } else {
        mFallFrame = al::getActionFrame(mActor);
    }

    al::addVelocityToGravity(mActor, mParam.gravity);
    al::scaleVelocityHV(mActor, 0.95f, 0.94f);
    if (isTriggerHacker(mHacker)) {
        al::setVelocityZeroV(mActor);
        al::LiveActor* actor = mActor;
        if (rs::isTriggerHackSwing(*mHacker)) {
            al::setActionFrameRate(actor, mParam.swingFramerate);
            mAccel = mParam.swingAccel;
            mVelocityY = mParam.swingVelocityY;
        } else {
            al::setActionFrameRate(actor, mParam.defaultFramerate);
            mAccel = mParam.defaultAccel;
            mVelocityY = mParam.defaultVelocityY;
        }
        al::setNerve(this, &NrvHostType.FallFly);
        return;
    }

    if (isOnGround()) {
        kill();
        return;
    }
    updateMove();
}

void HackerStateWingFly::exeFallFly() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "FallFly");

    bool isLessStep = al::isLessStep(this, 20);
    al::LiveActor* actor = mActor;
    if (!isLessStep) {
        al::addVelocityToGravity(actor, mParam.gravity);
        actor = mActor;
    }

    al::scaleVelocityHV(actor, 0.95f, 0.94f);
    if (isTriggerHacker(mHacker)) {
        al::Nerve* nerve;
        if (al::getTrans(mActor).y < mFlyLimit.y)
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
        al::startAction(actor, mParam.actionTrample);
        al::setVelocityZeroV(actor);
        al::addVelocityJump(actor, 20.0f);
    }

    if (al::isActionEnd(actor)) {
        al::setNerve(this, &NrvHostType.Fall);
        return;
    }

    if (isTriggerHacker(mHacker)) {
        if (rs::isTriggerHackSwing(*mHacker)) {
            al::setActionFrameRate(actor, mParam.swingFramerate);
            mAccel = mParam.swingAccel;
            mVelocityY = mParam.swingVelocityY;
        } else {
            al::setActionFrameRate(actor, mParam.defaultFramerate);
            mAccel = mParam.defaultAccel;
            mVelocityY = mParam.defaultVelocityY;
        }

        al::Nerve* nerve;
        if (al::getTrans(mActor).y < mFlyLimit.y)
            nerve = &NrvHostType.FlyRiseToHighest;
        else
            nerve = &NrvHostType.FlyRiseToTop;

        al::setNerve(this, nerve);
        return;
    }

    if (isOnGround()) {
        kill();
        return;
    }
    al::addVelocityToGravity(actor, mParam.gravity);
    al::scaleVelocityHV(actor, 0.95f, 0.94f);
    updateMove();
}

void HackerStateWingFly::exeUpperPunch() {
    al::LiveActor* actor = mActor;
    if (al::isFirstStep(this))
        al::startAction(actor, mParam.actionUpperPunch);

    if (al::isActionEnd(actor)) {
        al::setNerve(this, &NrvHostType.Fall);
        return;
    }

    if (al::isGreaterStep(this, 1) && isTriggerHacker(mHacker)) {
        if (rs::isTriggerHackSwing(*mHacker)) {
            al::setActionFrameRate(actor, mParam.swingFramerate);
            mAccel = mParam.swingAccel;
            mVelocityY = mParam.swingVelocityY;
        } else {
            al::setActionFrameRate(actor, mParam.defaultFramerate);
            mAccel = mParam.defaultAccel;
            mVelocityY = mParam.defaultVelocityY;
        }

        al::Nerve* nerve;
        if (al::getTrans(mActor).y < mFlyLimit.y)
            nerve = &NrvHostType.FlyRiseToHighest;
        else
            nerve = &NrvHostType.FlyRiseToTop;

        al::setNerve(this, nerve);
        return;
    }

    al::addVelocityToGravity(actor, mParam.gravity);
    al::scaleVelocityHV(actor, 0.95f, 0.94f);
    updateMove();
}
