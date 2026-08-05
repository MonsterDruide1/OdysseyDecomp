#include "ModeBalloon/TimeBalloonCircle.h"

#include "Library/Joint/JointControllerKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Npc/DecalJointCtrl.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(TimeBalloonCircle, Wait)
NERVE_IMPL(TimeBalloonCircle, In)
NERVE_IMPL(TimeBalloonCircle, Out)

NERVES_MAKE_NOSTRUCT(TimeBalloonCircle, Wait, In, Out)
}  // namespace

TimeBalloonCircle::TimeBalloonCircle() : al::LiveActor("TimeBalloonCircle") {}

void TimeBalloonCircle::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "TimeBalloonCircle", nullptr);
    al::initNerve(this, &Wait, 0);
    al::invalidateHitSensors(this);
    al::initJointControllerKeeper(this, 25);
    mDecalJointCtrl = new DecalJointCtrl(this, 25);
    makeActorDead();
}

void TimeBalloonCircle::appear() {
    al::LiveActor::appear();
    al::setNerve(this, &Wait);
}

void TimeBalloonCircle::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (!al::sendMsgPush(other, self))
        rs::sendMsgPushToPlayer(other, self);
}

bool TimeBalloonCircle::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                   al::HitSensor* self) {
    if (rs::isMsgPlayerDisregardHomingAttack(message) ||
        rs::isMsgPlayerDisregardTargetMarker(message))
        return true;
    return al::isMsgPlayerDisregard(message);
}

void TimeBalloonCircle::setTrans(const sead::Vector3f& trans) {
    mTrans = trans;
    al::resetPosition(this, mTrans);
    mDecalJointCtrl->calc();
}

void TimeBalloonCircle::startStandby() {
    al::invalidateHitSensors(this);
    al::showModelIfHide(this);
    al::startAction(this, "In");
    al::setNerve(this, &In);
}

void TimeBalloonCircle::startPlay() {
    al::validateHitSensors(this);
    al::showModelIfHide(this);
    al::startAction(this, "Out");
    al::setNerve(this, &Out);
}

void TimeBalloonCircle::hideModel() {
    al::hideModelIfShow(this);
}

void TimeBalloonCircle::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void TimeBalloonCircle::exeOut() {
    if (al::isActionEnd(this))
        al::setNerve(this, &Wait);
}

void TimeBalloonCircle::exeIn() {
    if (al::isActionEnd(this))
        al::setNerve(this, &Wait);
}
