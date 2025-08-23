#include "Boss/Stacker/StackerStatePanic.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Boss/Stacker/Stacker.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(StackerStatePanic, Surprise)
NERVE_IMPL(StackerStatePanic, CapHit)
NERVE_IMPL(StackerStatePanic, Wait)
NERVE_IMPL(StackerStatePanic, RunStart)
NERVE_IMPL(StackerStatePanic, Run)
NERVE_IMPL(StackerStatePanic, Turn)

NERVES_MAKE_STRUCT(StackerStatePanic, Surprise, CapHit, Wait, RunStart, Run, Turn)
}  // namespace

StackerStatePanic::StackerStatePanic(Stacker* stacker)
    : ActorStateBase("パニック(帽子全ロスト)", stacker) {
    initNerve(&NrvStackerStatePanic.Surprise, 0);
}

void StackerStatePanic::appear() {
    al::LiveActor* actor = mActor;
    _30 = 0;
    _34 = 0;
    _3c = 1.0f;
    _38 = 0;
    f32 sensorRadius = al::getSensorRadius(actor, "Body");
    al::setSensorRadius(actor, "Body", sensorRadius * 1.2f);
    al::setNerve(this, &NrvStackerStatePanic.Surprise);
    ActorStateBase::appear();
}

void StackerStatePanic::kill() {
    al::LiveActor* actor = mActor;

    f32 sensorRadius = al::getSensorRadius(actor, "Body");
    al::setSensorRadius(actor, "Body", sensorRadius / 1.2f);
    NerveStateBase::kill();
}

void StackerStatePanic::control() {
    _34++;
    if (900 < _34 && !al::isNerve(this, &NrvStackerStatePanic.Run)) {
        _38 = 1;
        al::setVelocityZero(mActor);
        kill();
    }
    al::setScale(mActor, {_3c, _3c, _3c});
    f32 lerpVal = al::lerpValue(_3c, 1.0, 0.12);
    _3c = lerpVal;
}

void StackerStatePanic::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isSensorEnemyAttack(self)) {
        rs::sendMsgPushToPlayer(other, self);
        return;
    }
    if (al::isSensorName(self, "CapReflect") && al::sendMsgPush(other, self))
        _3c = 1.1;
}

bool StackerStatePanic::receiveMsg(const al::SensorMsg* self, al::HitSensor* other,
                                   al::HitSensor* hitSensor) {
    if (_30 < 1) {
        if (!al::isNerve(this, &NrvStackerStatePanic.Wait) &&
            !al::isNerve(this, &NrvStackerStatePanic.Surprise) &&
            !al::isNerve(this, &NrvStackerStatePanic.CapHit)) {
            if (!rs::isMsgCapAttack(self) && !rs::isMsgStackerCapBoostAttack(self))
                return false;
            _30 = 10;
            al::faceToTarget(mActor, rs::getPlayerPos(mActor));
            al::startHitReaction(mActor, "逃げ中帽子ヒット");
            rs::requestHitReactionToAttacker(self, hitSensor, other);
            al::startAction(mActor, "PanicStart");
            al::setNerve(this, &NrvStackerStatePanic.CapHit);
            return true;
        } else {
            if (!rs::isMsgCapReflect(self) && !rs::isMsgStackerCapBoostAttack(self))
                return false;
            _30 = 10;
            al::faceToTarget(mActor, rs::getPlayerPos(mActor));
            al::startHitReaction(mActor, "逃げ中帽子ヒット");
            rs::requestHitReactionToAttacker(self, hitSensor, other);
            al::startAction(mActor, "PanicCapHit");
        }
        al::setNerve(this, &NrvStackerStatePanic.CapHit);
        return true;
    }
    return false;
}

bool StackerStatePanic::receiveMsgDamage(const al::SensorMsg* sensorMsg, al::HitSensor* hitSensor,
                                         al::HitSensor* secondSensor) {
    if ((al::isMsgPlayerTrample(sensorMsg) ||
         rs::isMsgPlayerAndCapObjHipDropReflectAll(sensorMsg)) &&
        al::getVelocity(al::getSensorHost(hitSensor)).y < 0.0 &&
        al::getTrans(mActor).y + 100.0f < al::getTrans(al::getSensorHost(hitSensor)).y) {
        _3c = 1.0;
        al::startHitReaction(mActor, "踏まれ");

        return true;
    }

    return false;
}

void StackerStatePanic::setIsRunAway(bool isRunAway) {
    mIsRunAway = isRunAway;
}

void StackerStatePanic::exeSurprise() {
    al::LiveActor* actor = mActor;
    if (al::isFirstStep(this)) {
        al::setVelocityZero(actor);
        al::startAction(actor, "Surprise");
        al::setVelocityZero(actor);
        al::addVelocityToFront(actor, -35.0);
    }
    al::addVelocityToGravity(actor, 3.0);
    al::scaleVelocity(actor, 0.9);
    if (al::isActionEnd(actor)) {
        if (!mIsRunAway)
            al::setNerve(this, &NrvStackerStatePanic.Wait);
        else
            al::setNerve(this, &NrvStackerStatePanic.RunStart);
    }
}

void StackerStatePanic::exeWait() {
    al::LiveActor* actor = mActor;
    if (0 < _30)
        --_30;
    if (al::isFirstStep(this)) {
        al::setVelocityZero(actor);
        al::startAction(actor, "Panic");
    }
    if (mIsRunAway && al::isGreaterEqualStep(this, 0x50))
        al::setNerve(this, &NrvStackerStatePanic.RunStart);
}

void StackerStatePanic::exeRunStart() {
    al::LiveActor* actor = mActor;
    if (isFirstStep(this)) {
        al::setVelocityZero(actor);
        al::startAction(actor, "PanicRunStart");
    }
    if (0 < _30)
        --_30;
    const sead::Vector3f& playerPos = rs::getPlayerPos(actor);
    sead::Vector3f dirToActorH;
    al::calcDirToActorH(&dirToActorH, actor, playerPos);
    const sead::Vector3f dirToActorHMin = -dirToActorH;
    al::turnToDirection(actor, dirToActorHMin, 0.8);
    al::addVelocityToFront(actor, 0.75);
    al::addVelocityToGravity(actor, 3.0);
    al::scaleVelocity(actor, 0.9);
    if (!al::isActionEnd(actor)) {
        if (!al::isCollidedWallVelocity(actor))
            return;
        const sead::Vector3f& collidedWallNormal = al::getCollidedWallNormal(actor);
        _24 = collidedWallNormal;
        al::setNerve(this, &NrvStackerStatePanic.Turn);
    } else
        al::setNerve(this, &NrvStackerStatePanic.Run);
}

void StackerStatePanic::exeRun() {
    al::LiveActor* actor = mActor;
    if (isFirstStep(this))
        al::startAction(actor, "PanicRun");
    if (0 < _30)
        --_30;
    const sead::Vector3f& playerPos = rs::getPlayerPos(actor);
    sead::Vector3f dirToActorH;
    al::calcDirToActorH(&dirToActorH, actor, playerPos);
    const sead::Vector3f dirToActorHMin = -dirToActorH;
    al::turnToDirection(actor, dirToActorHMin, 0.8);
    al::addVelocityToFront(actor, 0.75);
    al::addVelocityToGravity(actor, 3.0);
    al::scaleVelocity(actor, 0.9);
    if (al::isCollidedWallVelocity(actor)) {
        const sead::Vector3f& collidedWallNormal = al::getCollidedWallNormal(actor);
        _24 = collidedWallNormal;
        al::setNerve(this, &NrvStackerStatePanic.Turn);
    }
}

void StackerStatePanic::exeTurn() {
    al::LiveActor* actor = mActor;
    if (isFirstStep(this))
        al::setVelocityZero(actor);
    if (0 < _30)
        --_30;
    al::turnToDirection(actor, _24, 3.0);
    sead::Vector3f frontDir;
    al::calcFrontDir(&frontDir, actor);
    if (al::isNearDirection(frontDir, _24, 0.01))
        al::setNerve(this, &NrvStackerStatePanic.Run);
}

void StackerStatePanic::exeCapHit() {
    al::LiveActor* actor = mActor;
    if (isFirstStep(this)) {
        al::setVelocityZero(actor);
        al::addVelocityToFront(actor, -0.0);
    }
    if (0 < _30)
        --_30;
    if (al::isActionEnd(actor))
        al::setNerve(this, &NrvStackerStatePanic.Wait);
}

StackerStatePanic::~StackerStatePanic() = default;
