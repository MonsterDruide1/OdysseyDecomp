#include "Boss/GolemClimb/GolemStandBreakState.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Boss/GolemClimb/GolemClimbWeakPoint.h"
#include "Boss/GolemClimb/GolemShoutState.h"
#include "Boss/GolemClimb/IUseGolemState.h"

namespace {
NERVE_IMPL(GolemStandBreakState, Damage);
NERVE_IMPL(GolemStandBreakState, Shout);
NERVE_IMPL(GolemStandBreakState, Recover);
NERVES_MAKE_NOSTRUCT(GolemStandBreakState, Damage, Shout, Recover);
}  // namespace

GolemStandBreakState::GolemStandBreakState(const char* name, IUseGolemState* golemState,
                                           GolemShoutState* shoutState)
    : al::HostStateBase<IUseGolemState>(name, golemState), mShoutState(shoutState) {
    initNerve(&Damage, 0);
}

void GolemStandBreakState::appear() {
    al::setVelocityZero(getHost()->getActor());
    al::startHitReaction(getHost()->getActor(), "弱点ヒット");
    al::setNerve(this, &Damage);
    NerveStateBase::appear();
}

void GolemStandBreakState::kill() {
    mWeakPoint = nullptr;
    NerveStateBase::kill();
}

void GolemStandBreakState::exeDamage() {
    if (al::isFirstStep(this)) {
        mWeakPoint->startBreak();
        al::startAction(getHost()->getActor(), "StandDamageLast");
    }
    if (mWeakPoint->isBreak())
        al::setNerve(this, &Shout);
}

void GolemStandBreakState::exeShout() {
    if (al::isFirstStep(this))
        mShoutState->appear();
    mShoutState->control();
    if (al::isStep(this, 120)) {
        mShoutState->kill();
        al::setNerve(this, &Recover);
    }
}

void GolemStandBreakState::exeRecover() {
    if (al::isFirstStep(this))
        al::startAction(getHost()->getActor(), "StandRecover");
    if (al::isActionEnd(getHost()->getActor()))
        kill();
}

void GolemStandBreakState::startBreak(GolemClimbWeakPoint* weakPoint) {
    mWeakPoint = weakPoint;
}
