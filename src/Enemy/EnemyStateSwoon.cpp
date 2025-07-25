#include "Enemy/EnemyStateSwoon.h"

#include <math/seadVector.h>

#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Item/ItemUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nature/WaterSurfaceFinder.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(EnemyStateSwoon, SwoonStart);
NERVE_IMPL(EnemyStateSwoon, SwoonStartFall);
NERVE_IMPL(EnemyStateSwoon, SwoonStartLand);
NERVE_IMPL(EnemyStateSwoon, SwoonLoop);
NERVE_IMPL(EnemyStateSwoon, SwoonEndSign);
NERVE_IMPL(EnemyStateSwoon, SwoonEnd);
NERVE_IMPL(EnemyStateSwoon, SwoonTrampled);

NERVES_MAKE_NOSTRUCT(EnemyStateSwoon, SwoonStart, SwoonTrampled);
NERVES_MAKE_STRUCT(EnemyStateSwoon, SwoonEnd, SwoonLoop, SwoonStartFall, SwoonStartLand,
                   SwoonEndSign);
}  // namespace

bool isNearWater(al::WaterSurfaceFinder* waterSurfaceFinder, al::LiveActor* actor) {
    if (!waterSurfaceFinder || !waterSurfaceFinder->isFoundSurface())
        return false;

    sead::Vector3f gravity = al::getGravity(actor);
    sead::Vector3f nextPosition = al::getTrans(actor) - gravity * 100.0f;
    sead::Vector3f direction = gravity * 800.0f;
    sead::Vector3f hitPos;
    if (!alCollisionUtil::getHitPosOnArrow(actor, &hitPos, nextPosition, direction, nullptr,
                                           nullptr) ||
        !(gravity.dot(hitPos - waterSurfaceFinder->getSurfacePosition()) < 0.0f)) {
        bool isNearWater = waterSurfaceFinder->isNearSurface(60.0f);
        if (gravity.dot(al::getVelocity(actor)) > 0.0)
            isNearWater = waterSurfaceFinder->getDistance() > -60.0f;
        return isNearWater;
    }
    return false;
}

EnemyStateSwoon::EnemyStateSwoon(al::LiveActor* actor, const char* startAnimName,
                                 const char* loopAnimName, const char* endAnimName,
                                 bool hasSubActors, bool hasStartLandAnimation)
    : al::ActorStateBase("気絶ステート", actor), mStartAnimName(startAnimName),
      mLoopAnimName(loopAnimName), mEndAnimName(endAnimName), mHasSubActors(hasSubActors),
      mHasStartLandAnimation(hasStartLandAnimation) {
    initNerve(&SwoonStart, 0);
}

void EnemyStateSwoon::appear() {
    al::NerveStateBase::appear();
    mIsLockOn = false;
    al::setNerve(this, &SwoonStart);
}

void EnemyStateSwoon::control() {
    if (_24 > 0)
        _24--;
}

bool EnemyStateSwoon::tryReceiveMsgAttack(const al::SensorMsg* message) {
    if (isDead())
        return rs::isMsgCapAttack(message);
    return false;
}

bool EnemyStateSwoon::tryReceiveMsgStartHack(const al::SensorMsg* message) {
    if (rs::isMsgStartHack(message)) {
        if (!mHasLockOnDelay && al::isNerve(this, &SwoonStart))
            return al::isGreaterEqualStep(this, 40);
        return true;
    }
    return false;
}

bool EnemyStateSwoon::tryReceiveMsgEndSwoon(const al::SensorMsg* message) {
    if (rs::isMsgCapCancelLockOn(message)) {
        if (!al::isNerve(this, &NrvEnemyStateSwoon.SwoonEnd))
            al::setNerve(this, &NrvEnemyStateSwoon.SwoonEnd);
        return true;
    }
    return false;
}

bool EnemyStateSwoon::tryReceiveMsgPressDown(const al::SensorMsg* message) {
    if (rs::isMsgPressDown(message))
        return !mIsLockOn;
    return false;
}

bool EnemyStateSwoon::tryReceiveMsgObjHipDropAll(const al::SensorMsg* message) {
    if (rs::isMsgPlayerAndCapObjHipDropAll(message))
        return !mIsLockOn;
    return false;
}

bool EnemyStateSwoon::tryReceiveMsgTrample(const al::SensorMsg* message) {
    if (al::isMsgPlayerTrample(message))
        return requestTrampled();
    return false;
}

bool EnemyStateSwoon::tryReceiveMsgTrample(const al::SensorMsg* message, const al::HitSensor* other,
                                           const al::HitSensor* self) {
    if (al::isMsgPlayerTrampleForCrossoverSensor(message, other, self))
        return tryReceiveMsgTrample(message);
    return false;
}

bool EnemyStateSwoon::tryReceiveMsgTrampleReflect(const al::SensorMsg* message) {
    if (al::isMsgPlayerTrampleReflect(message))
        return requestTrampled();
    return false;
}

bool EnemyStateSwoon::tryReceiveMsgTrampleReflect(const al::SensorMsg* message,
                                                  const al::HitSensor* other,
                                                  const al::HitSensor* self) {
    if (al::isMsgPlayerTrampleReflectForCrossoverSensor(message, other, self))
        return tryReceiveMsgTrampleReflect(message);
    return false;
}

bool EnemyStateSwoon::tryReceiveMsgObjHipDropReflect(const al::SensorMsg* message) {
    if (rs::isMsgPlayerAndCapObjHipDropReflectAll(message)) {
        al::setNerve(this, &SwoonTrampled);
        return true;
    }
    return false;
}

bool EnemyStateSwoon::tryReceiveMsgObjLeapFrog(const al::SensorMsg* message,
                                               const al::HitSensor* other,
                                               const al::HitSensor* self) {
    if (rs::isMsgPlayerObjLeapFrog(message) &&
        !al::isNearZero(al::getActorVelocity(other), 0.001f)) {
        if (al::getActorVelocity(other).dot(al::getActorGravity(self)) < 0.0f)
            return false;
        return requestTrampled();
    }
    return false;
}

bool EnemyStateSwoon::tryReceiveMsgEnableLockOn(const al::SensorMsg* message) {
    if (rs::isMsgCapEnableLockOn(message)) {
        mIsLockOn = true;
        return true;
    }
    return false;
}

bool EnemyStateSwoon::tryReceiveMsgStartLockOn(const al::SensorMsg* message) {
    if (rs::isMsgCapEnableLockOn(message))
        return mHasLockOnDelay;
    return false;
}

bool EnemyStateSwoon::requestTrampled() {
    if (!al::isNerve(this, &SwoonTrampled) || !al::isLessStep(this, 10)) {
        al::setNerve(this, &SwoonTrampled);
        return true;
    }
    return false;
}

void EnemyStateSwoon::initParams(s32 swoonDuration, const char* trampledAnimName) {
    mSwoonDuration = swoonDuration;
    if (trampledAnimName != nullptr)
        mTrampledAnimName = trampledAnimName;
}

void EnemyStateSwoon::initParams(const EnemyStateSwoonInitParam& initParam) {
    mStartAnimName = initParam.startAnimName;
    mLoopAnimName = initParam.loopAnimName;
    mEndAnimName = initParam.endAnimName;
    if (initParam.trampledAnimName)
        mTrampledAnimName = initParam.trampledAnimName;
    mStartFallAnimName = initParam.startFallAnimName;
    mStartLandAnimName = initParam.startLandAnimName;
    mEndSignAnimName = initParam.endSignAnimName;
    mNearWaterStartAnimName = initParam.nearWaterStartAnimName;
    mNearWaterLoopAnimName = initParam.nearWaterLoopAnimName;
    mNearWaterEndAnimName = initParam.nearWaterEndAnimName;
    mNearWaterStartLandAnimName = initParam.nearWaterStartLandAnimName;
    mNearWaterTrampledAnimName = initParam.nearWaterTrampledAnimName;
    mHitReactionAnimName = initParam.hitReactionAnimName;
    mHitReactionLandAnimName = initParam.hitReactionLandAnimName;
    mHasSubActors = initParam.hasSubActors;
    mHasStartLandAnimation = initParam.hasStartLandAnimation;
    mHasLockOnDelay = initParam.hasLockOnDelay;
    mSwoonDuration = initParam.swoonDuration;
    mEndSignDelay = initParam.endSignDelay;
    mIsCancelLoopOnProhibitedArea = initParam.isCancelLoopOnProhibitedArea;
}

const char* EnemyStateSwoon::getSwoonStartAnimName() const {
    if (isNearWater(mWaterSurfaceFinder, mActor) && mNearWaterStartAnimName)
        return mNearWaterStartAnimName;
    return mStartAnimName;
}

const char* EnemyStateSwoon::getSwoonStartLandAnimName() const {
    if (isNearWater(mWaterSurfaceFinder, mActor) && mNearWaterStartLandAnimName)
        return mNearWaterStartLandAnimName;
    return mStartLandAnimName;
}

bool EnemyStateSwoon::isOnGroundOrWaterSurface() const {
    if (isNearWater(mWaterSurfaceFinder, mActor))
        return true;
    return al::isOnGround(mActor, 0);
}

bool EnemyStateSwoon::tryStartHitReactionLand() {
    if (isNearWater(mWaterSurfaceFinder, mActor)) {
        if (mHitReactionLandAnimName) {
            al::startHitReaction(mActor, mHitReactionLandAnimName);
            return true;
        }
        return false;
    }

    if (mHitReactionAnimName) {
        al::startHitReaction(mActor, mHitReactionAnimName);
        return true;
    }

    return false;
}

const char* EnemyStateSwoon::getSwoonLoopAnimName() const {
    if (isNearWater(mWaterSurfaceFinder, mActor) && mNearWaterLoopAnimName)
        return mNearWaterLoopAnimName;
    return mLoopAnimName;
}

bool EnemyStateSwoon::isPlayingActionIncorrect() const {
    al::LiveActor* actor = mActor;
    if (mNearWaterLoopAnimName && al::isActionPlaying(actor, mNearWaterLoopAnimName) &&
        al::isOnGround(actor, 0))
        return true;

    if (mLoopAnimName && al::isActionPlaying(actor, mLoopAnimName) &&
        isNearWater(mWaterSurfaceFinder, mActor)) {
        return true;
    }
    return false;
}

const char* EnemyStateSwoon::getSwoonEndAnimName() const {
    if (isNearWater(mWaterSurfaceFinder, mActor) && mNearWaterEndAnimName)
        return mNearWaterEndAnimName;
    return mEndAnimName;
}

const char* EnemyStateSwoon::getSwoonTrampledAnimName() const {
    if (isNearWater(mWaterSurfaceFinder, mActor) && mNearWaterTrampledAnimName)
        return mNearWaterTrampledAnimName;
    return mTrampledAnimName;
}

void inline startAction(al::LiveActor* actor, const char* animName, bool hasSubActors) {
    al::startAction(actor, animName);
    if (hasSubActors) {
        s32 size = al::getSubActorNum(actor);
        for (s32 i = 0; i < size; i++)
            al::startAction(al::getSubActor(actor, i), animName);
    }
}

void inline tryStartAction(al::LiveActor* actor, const char* animName, bool hasSubActors) {
    al::tryStartAction(actor, animName);
    if (hasSubActors) {
        s32 size = al::getSubActorNum(actor);
        for (s32 i = 0; i < size; i++)
            al::tryStartAction(al::getSubActor(actor, i), animName);
    }
}

void EnemyStateSwoon::exeSwoonStart() {
    if (al::isFirstStep(this))
        startAction(mActor, getSwoonStartAnimName(), mHasSubActors);

    if (al::isActionEnd(mActor)) {
        if (mHasStartLandAnimation && al::isExistAction(mActor, getSwoonStartLandAnimName())) {
            if (!isNearWater(mWaterSurfaceFinder, mActor) && !al::isOnGround(mActor, 0)) {
                al::setNerve(this, &NrvEnemyStateSwoon.SwoonStartFall);
                return;
            }
            if (!isNearWater(mWaterSurfaceFinder, mActor)) {
                al::setNerve(this, &NrvEnemyStateSwoon.SwoonStartLand);
                return;
            }
        }
        al::setNerve(this, &NrvEnemyStateSwoon.SwoonLoop);
    }
}

void EnemyStateSwoon::exeSwoonStartFall() {
    if (al::isFirstStep(this))
        tryStartAction(mActor, mStartFallAnimName, mHasSubActors);

    if (isNearWater(mWaterSurfaceFinder, mActor) || al::isOnGround(mActor, 0)) {
        tryStartHitReactionLand();
        al::setNerve(this, &NrvEnemyStateSwoon.SwoonStartLand);
    }
}

void EnemyStateSwoon::exeSwoonStartLand() {
    if (al::isFirstStep(this))
        startAction(mActor, getSwoonStartLandAnimName(), mHasSubActors);

    if (al::isActionEnd(mActor))
        al::setNerve(this, &NrvEnemyStateSwoon.SwoonLoop);
}

void EnemyStateSwoon::exeSwoonLoop() {
    if (mIsCancelLoopOnProhibitedArea && al::isInAreaObj(mActor, "HackCancelSwoonProhibitedArea")) {
        kill();
        return;
    }

    if (al::isFirstStep(this))
        startAction(mActor, getSwoonLoopAnimName(), mHasSubActors);

    if (isPlayingActionIncorrect()) {
        kill();
        return;
    }

    if (al::isGreaterEqualStep(this, mSwoonDuration)) {
        if (mEndSignAnimName) {
            al::setNerve(this, &NrvEnemyStateSwoon.SwoonEndSign);
            return;
        }
        al::setNerve(this, &NrvEnemyStateSwoon.SwoonEnd);
    }
}

void EnemyStateSwoon::exeSwoonEndSign() {
    if (al::isFirstStep(this))
        startAction(mActor, mEndSignAnimName, mHasSubActors);

    if (al::isGreaterEqualStep(this, mEndSignDelay))
        al::setNerve(this, &NrvEnemyStateSwoon.SwoonEnd);
}

void EnemyStateSwoon::exeSwoonEnd() {
    if (al::isFirstStep(this))
        startAction(mActor, getSwoonEndAnimName(), mHasSubActors);

    if (al::isActionEnd(mActor))
        kill();
}

void EnemyStateSwoon::exeSwoonTrampled() {
    if (al::isFirstStep(this))
        startAction(mActor, getSwoonTrampledAnimName(), mHasSubActors);

    if (al::isActionEnd(mActor)) {
        if (mIsAppearItem) {
            al::appearItem(mActor);
            mIsAppearItem = false;
        }
        al::setNerve(this, &NrvEnemyStateSwoon.SwoonLoop);
    }
}
