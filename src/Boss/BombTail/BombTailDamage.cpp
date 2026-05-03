#include "Boss/BombTail/BombTailDamage.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_ACTION_IMPL(BombTailDamage, BeforeBattle)
NERVE_ACTION_IMPL(BombTailDamage, Delay)
NERVE_ACTION_IMPL(BombTailDamage, Appear)
NERVE_ACTION_IMPL(BombTailDamage, Wait)
NERVE_ACTION_IMPL(BombTailDamage, Disappear)
NERVE_ACTION_IMPL(BombTailDamage, AppearChance)
NERVE_ACTION_IMPL(BombTailDamage, WaitChance)
NERVE_ACTION_IMPL(BombTailDamage, DisappearChance)

NERVE_ACTIONS_MAKE_STRUCT(BombTailDamage, BeforeBattle, Delay, Appear, Wait, Disappear,
                          AppearChance, WaitChance, DisappearChance)
}  // namespace

BombTailDamage::BombTailDamage() : al::LiveActor("BombTailDamage") {}

void BombTailDamage::init(const al::ActorInitInfo& info) {
    al::initNerveAction(this, "BeforeBattle", &NrvBombTailDamage.collector, 0);
    al::initActorWithArchiveName(this, info, "BombTailDamage", nullptr);
    al::invalidateClipping(this);
    makeActorDead();
}

void BombTailDamage::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isDead(this))
        return;

    if (al::isNerve(this, NrvBombTailDamage.Delay.data()))
        return;

    if (al::isNerve(this, NrvBombTailDamage.Appear.data()))
        return;

    if (al::isNerve(this, NrvBombTailDamage.AppearChance.data()))
        return;

    if (!al::isSensorEnemyAttack(self))
        return;

    if (!al::isSensorPlayer(other))
        return;

    if (al::getVelocity(al::getSensorHost(other)).y > 0.0f)
        return;

    if (!rs::isPlayerCollidedGround(this))
        return;

    al::sendMsgEnemyAttackFire(other, self, nullptr);
}

bool BombTailDamage::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(message) || rs::isMsgPlayerDisregardHomingAttack(message))
        return true;

    return rs::isMsgPlayerDisregardTargetMarker(message);
}

void BombTailDamage::startAppear(const sead::Vector3f& trans, s32 delayStep, bool isChance) {
    const char* sensorName = "Attack";

    al::LiveActor::appear();
    al::setTrans(this, trans);
    al::resetPosition(this);
    mDelayStep = delayStep;
    mIsChance = isChance;

    if (isChance)
        al::setSensorRadius(this, sensorName, 227.5f);
    else
        al::setSensorRadius(this, sensorName, 130.0f);

    al::startNerveAction(this, "Delay");
}

void BombTailDamage::forceDisappear() {
    if (al::isDead(this))
        return;

    if (mIsChance)
        al::startNerveAction(this, "DisappearChance");
    else
        al::startNerveAction(this, "Disappear");
}

void BombTailDamage::exeBeforeBattle() {}

void BombTailDamage::exeDelay() {
    if (!al::isGreaterEqualStep(this, mDelayStep))
        return;

    if (mIsChance)
        al::startNerveAction(this, "AppearChance");
    else
        al::startNerveAction(this, "Appear");
}

void BombTailDamage::exeAppear() {
    if (al::isGreaterEqualStep(this, 30))
        al::startNerveAction(this, "Wait");
}

void BombTailDamage::exeWait() {
    if (al::isGreaterEqualStep(this, 800))
        al::startNerveAction(this, "Disappear");
}

void BombTailDamage::exeDisappear() {
    if (al::isGreaterEqualStep(this, 15))
        kill();
}

void BombTailDamage::exeAppearChance() {
    if (al::isGreaterEqualStep(this, 30))
        al::startNerveAction(this, "WaitChance");
}

void BombTailDamage::exeWaitChance() {
    if (al::isGreaterEqualStep(this, 800))
        al::startNerveAction(this, "DisappearChance");
}

void BombTailDamage::exeDisappearChance() {
    if (al::isGreaterEqualStep(this, 15))
        kill();
}
