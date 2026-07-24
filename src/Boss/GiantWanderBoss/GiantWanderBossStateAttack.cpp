#include "Boss/GiantWanderBoss/GiantWanderBossStateAttack.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Boss/GiantWanderBoss/GiantWanderBossBullet.h"
#include "Boss/GiantWanderBoss/GiantWanderBossMine.h"

namespace {
NERVE_IMPL(GiantWanderBossStateAttack, AttackSign)
NERVE_IMPL(GiantWanderBossStateAttack, AttackSignWait)
NERVE_IMPL(GiantWanderBossStateAttack, AttackStart)
NERVE_IMPL(GiantWanderBossStateAttack, Attack)
NERVE_IMPL(GiantWanderBossStateAttack, AttackEnd)

NERVES_MAKE_NOSTRUCT(GiantWanderBossStateAttack, AttackSign, AttackSignWait, AttackStart, Attack,
                     AttackEnd)
}  // namespace

GiantWanderBossStateAttack::GiantWanderBossStateAttack(al::LiveActor* actor)
    : al::ActorStateBase("徘徊ボス攻撃", actor) {
    initNerve(&AttackSign, 0);
}

void GiantWanderBossStateAttack::appear() {
    NerveStateBase::appear();
    al::setNerve(this, &AttackSign);
}

void GiantWanderBossStateAttack::kill() {
    NerveStateBase::kill();

    if (mBullet)
        mBullet->startLaunch();

    if (mMine)
        mMine->startLaunchForFirstPhase();
}

void GiantWanderBossStateAttack::startWithBullet(GiantWanderBossBullet* bullet) {
    mBullet = bullet;
    mMine = nullptr;
    al::setNerve(this, &AttackSign);
}

void GiantWanderBossStateAttack::startWithMineFirstPhase(GiantWanderBossMine* mine) {
    mBullet = nullptr;
    mMine = mine;
    mMineAttackType = 1;
    al::setNerve(this, &AttackSign);
}

void GiantWanderBossStateAttack::startWithMineEscape(GiantWanderBossMine* mine) {
    mBullet = nullptr;
    mMine = mine;
    mMineAttackType = 2;
    al::setNerve(this, &AttackSign);
}

void GiantWanderBossStateAttack::startWithMineLongRange(GiantWanderBossMine* mine) {
    mBullet = nullptr;
    mMine = mine;
    mMineAttackType = 3;
    al::setNerve(this, &AttackSign);
}

void GiantWanderBossStateAttack::exeAttackSign() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "AttackSign");

        if (mMine)
            al::startAction(mMine, "AttackSign");

        if (mBullet)
            al::startAction(mBullet, "AttackSign");
    }

    if (al::isActionEnd(mActor))
        al::setNerve(this, &AttackSignWait);
}

void GiantWanderBossStateAttack::exeAttackSignWait() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "AttackSignWait");

        if (mMine)
            al::startAction(mMine, "AttackSignWait");

        if (mBullet)
            al::startAction(mBullet, "AttackSignWait");
    }

    if (al::isGreaterEqualStep(this, 30))
        al::setNerve(this, &AttackStart);
}

void GiantWanderBossStateAttack::exeAttackStart() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "AttackStart");

        if (mMine)
            al::startAction(mMine, "AttackStart");

        if (mBullet)
            al::startAction(mBullet, "AttackStart");
    }

    if (al::isActionEnd(mActor))
        al::setNerve(this, &Attack);
}

void GiantWanderBossStateAttack::exeAttack() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "Attack");

        if (mBullet) {
            mBullet->startLaunch();
            mBullet = nullptr;
        } else {
            GiantWanderBossMine** mine;

            switch (mMineAttackType) {
            case 3:
                mine = &mMine;
                mMine->startLaunchForLongRange();
                break;
            case 1:
                mine = &mMine;
                mMine->startLaunchForFirstPhase();
                break;
            case 2:
                mine = &mMine;
                mMine->startLaunchForEscape();
                break;
            default:
                mine = &mMine;
                break;
            }

            *mine = nullptr;
        }
    }

    if (al::isActionEnd(mActor))
        al::setNerve(this, &AttackEnd);
}

void GiantWanderBossStateAttack::exeAttackEnd() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "AttackEnd");

    if (al::isActionEnd(mActor)) {
        al::NerveStateBase* state = this;
        state->kill();
    }
}
