#include "Enemy/JokuHand.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(JokuHand, Wait);
NERVE_IMPL(JokuHand, Damage);
NERVES_MAKE_NOSTRUCT(JokuHand, Wait, Damage);
}  // namespace

JokuHand::JokuHand(const char* name) : al::LiveActor(name) {}

void JokuHand::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "JokuHand", nullptr);
    al::initNerve(this, &Wait, 0);
    al::invalidateClipping(this);
    makeActorDead();
}

void JokuHand::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isSensorName(self, "Attack"))
        al::sendMsgEnemyAttack(other, self) || al::sendMsgExplosion(other, self, nullptr);
    if (al::isSensorName(self, "Body"))
        al::sendMsgPush(other, self);
}

bool JokuHand::receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self) {
    if (rs::isMsgPlayerDisregardHomingAttack(msg) || rs::isMsgPlayerDisregardTargetMarker(msg) ||
        al::isMsgPlayerDisregard(msg))
        return true;
    return false;
}

void JokuHand::control() {
    al::updatePoseMtx(this, mMtx);
}

void JokuHand::appear() {
    al::setNerve(this, &Wait);
    al::LiveActor::appear();
    al::updatePoseMtx(this, mMtx);
}

void JokuHand::damage() {
    al::setNerve(this, &Damage);
}

void JokuHand::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void JokuHand::exeDamage() {
    if (al::isFirstStep(this))
        al::startHitReaction(this, "ダメージ");
    if (al::isGreaterStep(this, 5))
        kill();
}
