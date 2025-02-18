#include "Enemy/FlyerStateWander.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorParamMove.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(FlyerStateWander, Wander)
NERVE_IMPL(FlyerStateWander, Wait)

NERVES_MAKE_NOSTRUCT(FlyerStateWander, Wander, Wait)
}  // namespace

FlyerStateWanderParam::FlyerStateWanderParam(s32 ukn, s32 wanderTime, s32 waitTime,
                                             const char* actionName,
                                             const al::ActorParamMove* actorParamMove)
    : _0(ukn), mWanderTime(wanderTime), mWaitTime(waitTime), mActionName(actionName),
      mActorParamMove(actorParamMove) {}

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
        mStartTrans.set(al::getTrans(mActor));
        al::startAction(mActor, mFlyerStateWanderParam->getActionName());
        mNerveTime = mFlyerStateWanderParam->getWanderTime() +
                     mFlyerStateWanderParam->get_0() * al::getRandom(3);
    }

    const al::ActorParamMove* actorParamMove = mFlyerStateWanderParam->getActorParamMove();
    al::flyAndTurnToTarget(mActor, mStartTrans, actorParamMove->_0, actorParamMove->_4,
                           actorParamMove->_8, actorParamMove->_c);

    if (al::isGreaterEqualStep(this, mNerveTime))
        al::setNerve(this, &Wait);
}

void FlyerStateWander::exeWait() {
    if (al::isFirstStep(this)) {
        mNerveTime = mFlyerStateWanderParam->getWaitTime() +
                     mFlyerStateWanderParam->get_0() * al::getRandom(3);
    }

    al::setVelocityZero(mActor);

    if (al::isGreaterEqualStep(this, mNerveTime))
        al::setNerve(this, &Wander);
}
