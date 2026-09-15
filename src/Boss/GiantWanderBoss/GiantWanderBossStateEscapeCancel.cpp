#include "Boss/GiantWanderBoss/GiantWanderBossStateEscapeCancel.h"

#include <math/seadVector.h>

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Boss/GiantWanderBoss/GiantWanderBoss.h"
#include "Boss/GiantWanderBoss/GiantWanderBossMine.h"

namespace {
NERVE_IMPL(GiantWanderBossStateEscapeCancel, Angry)
NERVE_IMPL(GiantWanderBossStateEscapeCancel, AppearMine)
NERVE_IMPL(GiantWanderBossStateEscapeCancel, Launch)
NERVE_IMPL(GiantWanderBossStateEscapeCancel, LaunchEnd)

NERVES_MAKE_NOSTRUCT(GiantWanderBossStateEscapeCancel, Angry, AppearMine, Launch, LaunchEnd)

const sead::Vector3f sMineAttachOffset = sead::Vector3f::zero;
}  // namespace

GiantWanderBossStateEscapeCancel::GiantWanderBossStateEscapeCancel(
    GiantWanderBoss* boss, al::DeriveActorGroup<GiantWanderBossMine>* mineGroup)
    : al::HostStateBase<GiantWanderBoss>("逃げキャンセル", boss), mMineGroup(mineGroup) {
    initNerve(&Angry, 0);
}

GiantWanderBossStateEscapeCancel::~GiantWanderBossStateEscapeCancel() = default;

void GiantWanderBossStateEscapeCancel::appear() {
    NerveStateBase::appear();
    mLaunchCount = 0;
    al::setNerve(this, &Angry);
}

void GiantWanderBossStateEscapeCancel::exeAngry() {
    if (al::isFirstStep(this)) {
        al::setVelocityZero(getHost());
        al::startAction(getHost(), "Angry");
    }

    if (al::isActionEnd(getHost()))
        al::setNerve(this, &AppearMine);
}

void GiantWanderBossStateEscapeCancel::exeAppearMine() {
    if (al::isFirstStep(this)) {
        mMine = mMineGroup->tryFindDeadDeriveActor();

        if (!mMine) {
            al::NerveStateBase* state = this;
            state->kill();
            return;
        }

        mMine->appearAttach(getHost()->getBulletMtx(), &sMineAttachOffset);
        al::startAction(getHost(), "AttackSign");
    }

    if (al::isActionEnd(getHost()))
        al::setNerve(this, &Launch);
}

void GiantWanderBossStateEscapeCancel::exeLaunch() {
    if (al::isFirstStep(this)) {
        al::startAction(getHost(), "Attack");
        mMine->startLaunchForFirstPhase();
        mLaunchCount++;
    }

    al::turnFront(getHost(), 3.0f);

    if (!al::isGreaterEqualStep(this, 20))
        return;

    if (mLaunchCount >= 6) {
        al::setNerve(this, &LaunchEnd);
        return;
    }

    mMine = mMineGroup->tryFindDeadDeriveActor();

    if (!mMine) {
        al::setNerve(this, &LaunchEnd);
        return;
    }

    mMine->appearAttach(getHost()->getBulletMtx(), &sMineAttachOffset);
    al::setNerve(this, &Launch);
}

void GiantWanderBossStateEscapeCancel::exeLaunchEnd() {
    if (al::isFirstStep(this))
        al::startAction(getHost(), "AttackEnd");

    if (al::isActionEnd(getHost())) {
        al::NerveStateBase* state = this;
        state->kill();
    }
}
