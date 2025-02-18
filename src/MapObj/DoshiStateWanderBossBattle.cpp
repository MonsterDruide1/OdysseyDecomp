#include "MapObj/DoshiStateWanderBossBattle.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "MapObj/Doshi.h"

namespace {
NERVE_IMPL(DoshiStateWanderBossBattle, Wait);

NERVES_MAKE_STRUCT(DoshiStateWanderBossBattle, Wait);
}  // namespace

DoshiStateWanderBossBattle::DoshiStateWanderBossBattle(Doshi* doshi)
    : al::HostStateBase<Doshi>("着替えデモステート", doshi) {
    initNerve(&NrvDoshiStateWanderBossBattle.Wait, 0);
}

void DoshiStateWanderBossBattle::appear() {
    setDead(false);
    al::setNerve(this, &NrvDoshiStateWanderBossBattle.Wait);
}

bool DoshiStateWanderBossBattle::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                            al::HitSensor* self) {
    return false;
}

void DoshiStateWanderBossBattle::setting(const sead::Vector3f& position,
                                         const sead::Vector3f& front) {
    mPosition = position;
    mFront = front;
}

void DoshiStateWanderBossBattle::exeWait() {
    if (al::isFirstStep(this)) {
        al::resetPosition(getHost(), mPosition);
        al::setFront(getHost(), mFront);
        al::tryStartActionIfNotPlaying(getHost(), "Wait");
        al::validateClipping(getHost());
    }
}

void DoshiStateWanderBossBattle::exeReaction() {}
