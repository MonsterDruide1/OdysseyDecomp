#include "Enemy/FlyerStateWander.h"

#include "Library/Nerve/NerveSetupUtil.h"

namespace {
NERVE_IMPL(FlyerStateWander, Wander)
NERVE_IMPL(FlyerStateWander, Wait)

NERVES_MAKE_STRUCT(FlyerStateWander, Wander, Wait)
}  // namespace

FlyerStateWander::FlyerStateWander(al::LiveActor* actor, const FlyerStateWanderParam* param)
    : al::ActorStateBase("飛行型うろつき状態", actor), mFlyerStateWanderParam(param) {
    initNerve(&NrvFlyerStateWander.Wander, 0);
}

void FlyerStateWander::appear() {
    setDead(false);
    al::setNerve(this, &NrvFlyerStateWander.Wander);
}
