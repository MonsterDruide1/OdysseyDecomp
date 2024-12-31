#include "Enemy/FlyerStateWander.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathRandomUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"

#include "Enemy/FlyerStateWanderParam.h"

namespace {
NERVE_IMPL(FlyerStateWander, Wander)
NERVE_IMPL(FlyerStateWander, Wait)

NERVES_MAKE_NOSTRUCT(FlyerStateWander, Wander, Wait)
}  // namespace

FlyerStateWander::FlyerStateWander(al::LiveActor* actor, const FlyerStateWanderParam* param)
    : al::ActorStateBase("飛行型うろつき状態", actor), mFlyerStateWanderParam(param) {
    initNerve(&Wander, 0);
}

void FlyerStateWander::appear() {
    setDead(false);
    al::setNerve(this, &Wander);
}

void FlyerStateWander::exeWander() {
    if (al::isFirstStep(this)) {
        _24.set(al::getTrans(mActor));
        al::startAction(mActor, mFlyerStateWanderParam->getActionName());
        s32 iVar1 = mFlyerStateWanderParam->get_4();
        s32 iVar2 = mFlyerStateWanderParam->get_0();
        _20 = iVar1 + al::getRandom(3) * iVar2;
    }

    al::ActorParamMove* actorParamMove = mFlyerStateWanderParam->getActorParamMove();
    al::flyAndTurnToTarget(mActor, _24, actorParamMove->ukn[0], actorParamMove->ukn[1],
                           actorParamMove->ukn[2], actorParamMove->ukn[3]);

    if (al::isGreaterEqualStep(this, _20))
        al::setNerve(this, &Wait);
}

void FlyerStateWander::exeWait() {
    if (al::isFirstStep(this)) {
        s32 iVar1 = mFlyerStateWanderParam->get_8();
        s32 iVar2 = mFlyerStateWanderParam->get_0();
        _20 = iVar1 + al::getRandom(3) * iVar2;
    }

    al::setVelocityZero(mActor);

    if (al::isGreaterEqualStep(this, _20))
        al::setNerve(this, &Wander);
}
