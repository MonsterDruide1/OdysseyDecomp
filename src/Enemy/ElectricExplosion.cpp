#include "Enemy/ElectricExplosion.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(ElectricExplosion, Wait)
NERVE_IMPL(ElectricExplosion, Attack)

NERVES_MAKE_NOSTRUCT(ElectricExplosion, Wait, Attack)

const f32 sAttackSensorRadiusTable[] = {
    100.0f, 170.0f, 240.0f, 310.0f, 380.0f, 450.0f, 520.0f, 590.0f, 660.0f, 730.0f,
};
}  // namespace

ElectricExplosion::ElectricExplosion(const char* name, al::LiveActor* hostActor)
    : al::LiveActor(name), mHostActor(hostActor) {}

void ElectricExplosion::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "ElectricExplosion", nullptr);
    al::initNerve(this, &Wait, 0);
    al::setTrans(this, al::getTrans(mHostActor));
    makeActorDead();
}

void ElectricExplosion::attack(s32 attackLevel) {
    if (al::isNerve(this, &Attack))
        return;

    al::setNerve(this, &Attack);
    mAttackLevel = attackLevel;
    al::setTrans(this, al::getTrans(mHostActor));
    al::setSensorRadius(this, "Attack", sAttackSensorRadiusTable[mAttackLevel - 1]);
    al::validateHitSensors(this);
    appear();
}

void ElectricExplosion::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    rs::sendMsgHackAttack(other, self);
}

void ElectricExplosion::exeWait() {}

void ElectricExplosion::exeAttack() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Attack");
        al::startAction(this, "Level");
    }

    al::setTrans(this, al::getTrans(mHostActor));
    al::setVisAnimFrameForAction(this, mAttackLevel - 1);
    if (al::isGreaterEqualStep(this, 20)) {
        al::setNerve(this, &Wait);
        kill();
    }
}
