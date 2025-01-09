#include "MapObj/DoshiStateWanderBossBattle.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "MapObj/Doshi.h"

namespace {
NERVE_IMPL(DoshiStateWanderBossBattle, Wait);
NERVE_IMPL(DoshiStateWanderBossBattle, Reaction);

NERVES_MAKE_STRUCT(DoshiStateWanderBossBattle, Wait, Reaction);
}  // namespace

DoshiStateWanderBossBattle::DoshiStateWanderBossBattle(Doshi* doshi)
    : NerveStateBase("着替えデモステート"), mDoshi(doshi) {
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
        al::resetPosition(mDoshi, mPosition);
        al::setFront(mDoshi, mFront);
        al::tryStartActionIfNotPlaying(mDoshi, "Wait");
        al::validateClipping(mDoshi);
    }
}

void DoshiStateWanderBossBattle::exeReaction() {
    return;
}
