#include "Enemy/JokuArm.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(JokuArm, Wait)
NERVE_IMPL(JokuArm, Damage)
NERVES_MAKE_NOSTRUCT(JokuArm, Wait, Damage)
}  // namespace

JokuArm::JokuArm(const char* name) : al::LiveActor(name) {}

void JokuArm::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "JokuArm", nullptr);
    al::initNerve(this, &Wait, 0);
    al::invalidateClipping(this);
    makeActorDead();
}

void JokuArm::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isSensorName(self, "Attack"))
        al::sendMsgEnemyAttack(other, self) || al::sendMsgExplosion(other, self, nullptr);
    if (al::isSensorName(self, "Body"))
        al::sendMsgPush(other, self);
}

bool JokuArm::receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self) {
    if (rs::isMsgPlayerDisregardHomingAttack(msg))
        return true;
    return rs::isMsgPlayerDisregardTargetMarker(msg);
}

void JokuArm::control() {
    al::updatePoseMtx(this, mJointMtx);
}

void JokuArm::appear() {
    al::setNerve(this, &Wait);
    al::LiveActor::appear();
    al::updatePoseMtx(this, mJointMtx);
    al::hideModelIfShow(this);
}

void JokuArm::damage() {
    al::setNerve(this, &Damage);
}

void JokuArm::exeWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
        al::showModelIfHide(this);
    }
}

void JokuArm::exeDamage() {
    if (al::isFirstStep(this))
        al::startHitReaction(this, "ダメージ");
    kill();
}
