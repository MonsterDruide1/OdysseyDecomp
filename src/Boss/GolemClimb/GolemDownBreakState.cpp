#include "Boss/GolemClimb/GolemDownBreakState.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Boss/GolemClimb/GolemClimbWeakPoint.h"
#include "Boss/GolemClimb/GolemShoutState.h"
#include "Boss/GolemClimb/IUseGolemState.h"

namespace {
NERVE_IMPL(GolemDownBreakState, Damage);
NERVE_IMPL(GolemDownBreakState, Shout);
NERVE_IMPL(GolemDownBreakState, Recover);
NERVES_MAKE_NOSTRUCT(GolemDownBreakState, Damage, Shout, Recover);
}  // namespace

GolemDownBreakState::GolemDownBreakState(const char* name, IUseGolemState* golemState,
                                         GolemShoutState* shoutState)
    : al::HostStateBase<IUseGolemState>(name, golemState), mShoutState(shoutState) {
    initNerve(&Damage, 0);
}

void GolemDownBreakState::appear() {
    al::setVelocityZero(getHost()->getActor());
    al::startHitReaction(getHost()->getActor(), "弱点ヒット");
    al::setNerve(this, &Damage);
    NerveStateBase::appear();
}

void GolemDownBreakState::kill() {
    mWeakPoint = nullptr;
    mDamageActionName = nullptr;
    mRecoverActionName = nullptr;
    getHost()->endPushSensor();
    NerveStateBase::kill();
}

void GolemDownBreakState::exeDamage() {
    if (al::isFirstStep(this)) {
        mWeakPoint->startBreak();
        al::startAction(getHost()->getActor(), mDamageActionName);
    }
    if (mWeakPoint->isBreak())
        al::setNerve(this, &Shout);
}

void GolemDownBreakState::exeShout() {
    if (al::isFirstStep(this))
        mShoutState->appear();
    mShoutState->control();
    if (al::isStep(this, 120)) {
        mShoutState->kill();
        al::setNerve(this, &Recover);
    }
}

void GolemDownBreakState::exeRecover() {
    if (al::isFirstStep(this))
        al::startAction(getHost()->getActor(), mRecoverActionName);
    getHost()->updatePushSensor();
    if (al::isActionEnd(getHost()->getActor()) && getHost()->tryNextPushSensor())
        kill();
}

void GolemDownBreakState::startBreak(GolemClimbWeakPoint* weakPoint, const char* damageActionName,
                                     const char* recoverActionName) {
    mWeakPoint = weakPoint;
    mDamageActionName = damageActionName;
    mRecoverActionName = recoverActionName;
}
