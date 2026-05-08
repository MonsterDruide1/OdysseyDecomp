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
    mInvincibleCounter = 0;
    mPanicCounter = 0;
    mScale = 1.0f;
    mIsKillByTimer = false;
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
    mPanicCounter++;
    if (mPanicCounter > 900 && !al::isNerve(this, &NrvStackerStatePanic.CapHit)) {
        mIsKillByTimer = true;
        al::setVelocityZero(mActor);
        kill();
    }
    al::setScale(mActor, {mScale, mScale, mScale});
    mScale = al::lerpValue(mScale, 1.0f, 0.12f);
}

void StackerStatePanic::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isSensorEnemyAttack(self)) {
        rs::sendMsgPushToPlayer(other, self);
        return;
    }
    if (al::isSensorName(self, "CapReflect") && al::sendMsgPush(other, self))
        mScale = 1.1f;
}

bool StackerStatePanic::receiveMsg(const al::SensorMsg* self, al::HitSensor* other,
                                   al::HitSensor* hitSensor) {
    if (mInvincibleCounter > 0)
        return false;

    if (al::isNerve(this, &NrvStackerStatePanic.Wait) ||
        al::isNerve(this, &NrvStackerStatePanic.Surprise) ||
        al::isNerve(this, &NrvStackerStatePanic.CapHit)) {
        if (rs::isMsgCapReflect(self) || rs::isMsgStackerCapBoostAttack(self)) {
            mInvincibleCounter = 10;
            al::faceToTarget(mActor, rs::getPlayerPos(mActor));
            al::startHitReaction(mActor, "逃げ中帽子ヒット");
            rs::requestHitReactionToAttacker(self, hitSensor, other);
            al::startAction(mActor, "PanicCapHit");
            al::setNerve(this, &NrvStackerStatePanic.CapHit);
            return true;
        }
    } else if (rs::isMsgCapAttack(self) || rs::isMsgStackerCapBoostAttack(self)) {
        mInvincibleCounter = 10;
        al::faceToTarget(mActor, rs::getPlayerPos(mActor));
        al::startHitReaction(mActor, "逃げ中帽子ヒット");
        rs::requestHitReactionToAttacker(self, hitSensor, other);
        al::startAction(mActor, "PanicStart");
        al::setNerve(this, &NrvStackerStatePanic.CapHit);
        return true;
    }
    return false;
}

bool StackerStatePanic::receiveMsgDamage(const al::SensorMsg* self, al::HitSensor* other,
                                         al::HitSensor* _hitSensor) {
    if ((al::isMsgPlayerTrample(self) || rs::isMsgPlayerAndCapObjHipDropReflectAll(self)) &&
        al::getVelocity(al::getSensorHost(other)).y < 0.0f &&
        al::getTrans(mActor).y + 100.0f < al::getTrans(al::getSensorHost(other)).y) {
        mScale = 1.0f;
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
        al::addVelocityToFront(actor, -35.0f);
    }
    al::addVelocityToGravity(actor, 3.0f);
    al::scaleVelocity(actor, 0.9f);
    if (al::isActionEnd(actor)) {
        if (mIsRunAway)
            al::setNerve(this, &NrvStackerStatePanic.RunStart);
        else
            al::setNerve(this, &NrvStackerStatePanic.Wait);
    }
}

void StackerStatePanic::exeWait() {
    al::LiveActor* actor = mActor;
    if (mInvincibleCounter > 0)
        mInvincibleCounter--;
    if (al::isFirstStep(this)) {
        al::setVelocityZero(actor);
        al::startAction(actor, "Panic");
    }
    if (mIsRunAway && al::isGreaterEqualStep(this, 80))
        al::setNerve(this, &NrvStackerStatePanic.RunStart);
}

void StackerStatePanic::exeRunStart() {
    al::LiveActor* actor = mActor;
    if (al::isFirstStep(this)) {
        al::setVelocityZero(actor);
        al::startAction(actor, "PanicRunStart");
    }
    if (mInvincibleCounter > 0)
        mInvincibleCounter--;

    sead::Vector3f dirToActorH;
    al::calcDirToActorH(&dirToActorH, actor, rs::getPlayerPos(actor));
    const sead::Vector3f targetDirection = -dirToActorH;
    al::turnToDirection(actor, targetDirection, 0.8f);
    al::addVelocityToFront(actor, 0.75f);
    al::addVelocityToGravity(actor, 3.0f);
    al::scaleVelocity(actor, 0.9f);

    if (al::isActionEnd(actor)) {
        al::setNerve(this, &NrvStackerStatePanic.Run);
        return;
    }
    if (al::isCollidedWallVelocity(actor)) {
        mTurnTarget = al::getCollidedWallNormal(actor);
        al::setNerve(this, &NrvStackerStatePanic.Turn);
    }
}

void StackerStatePanic::exeRun() {
    al::LiveActor* actor = mActor;
    if (al::isFirstStep(this))
        al::startAction(actor, "PanicRun");
    if (mInvincibleCounter > 0)
        mInvincibleCounter--;
    sead::Vector3f dirToActorHMin;
    al::calcDirToActorH(&dirToActorHMin, actor, rs::getPlayerPos(actor));
    const sead::Vector3f targetDirection = -dirToActorHMin;
    al::turnToDirection(actor, targetDirection, 0.8f);
    al::addVelocityToFront(actor, 0.75f);
    al::addVelocityToGravity(actor, 3.0f);
    al::scaleVelocity(actor, 0.9f);
    if (al::isCollidedWallVelocity(actor)) {
        mTurnTarget = al::getCollidedWallNormal(actor);
        al::setNerve(this, &NrvStackerStatePanic.Turn);
    }
}

void StackerStatePanic::exeTurn() {
    al::LiveActor* actor = mActor;
    if (al::isFirstStep(this))
        al::setVelocityZero(actor);
    if (mInvincibleCounter > 0)
        mInvincibleCounter--;
    al::turnToDirection(actor, mTurnTarget, 3.0f);
    sead::Vector3f frontDir;
    al::calcFrontDir(&frontDir, actor);
    if (al::isNearDirection(frontDir, mTurnTarget, 0.01f))
        al::setNerve(this, &NrvStackerStatePanic.Run);
}

void StackerStatePanic::exeCapHit() {
    al::LiveActor* actor = mActor;
    if (al::isFirstStep(this)) {
        al::setVelocityZero(actor);
        al::addVelocityToFront(actor, -0.0f);
    }
    if (mInvincibleCounter > 0)
        mInvincibleCounter--;
    if (al::isActionEnd(actor))
        al::setNerve(this, &NrvStackerStatePanic.Wait);
}
