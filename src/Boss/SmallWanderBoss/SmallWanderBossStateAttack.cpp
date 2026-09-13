#include "Boss/SmallWanderBoss/SmallWanderBossStateAttack.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Boss/SmallWanderBoss/SmallWanderBossBullet.h"

namespace {
NERVE_IMPL(SmallWanderBossStateAttack, AttackSign);
NERVE_IMPL(SmallWanderBossStateAttack, AttackSignWait);
NERVE_IMPL(SmallWanderBossStateAttack, AttackStart);
NERVE_IMPL(SmallWanderBossStateAttack, Attack);
NERVE_IMPL(SmallWanderBossStateAttack, AttackEnd);

NERVES_MAKE_NOSTRUCT(SmallWanderBossStateAttack, AttackSign, AttackSignWait, AttackStart, Attack,
                     AttackEnd);
}  // namespace

SmallWanderBossStateAttack::SmallWanderBossStateAttack(al::LiveActor* actor)
    : al::ActorStateBase("ザコ徘徊攻撃", actor), mBullet(nullptr) {
    initNerve(&AttackSign, 0);
}

void SmallWanderBossStateAttack::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &AttackSign);
}

void SmallWanderBossStateAttack::kill() {
    al::NerveStateBase::kill();

    if (mBullet)
        mBullet->startLaunch();
}

void SmallWanderBossStateAttack::startWithBullet(SmallWanderBossBullet* bullet) {
    mBullet = bullet;
    al::setNerve(this, &AttackSign);
}

void SmallWanderBossStateAttack::exeAttackSign() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "AttackSign");
        al::startAction(mBullet, "AttackSign");
    }

    if (al::isActionEnd(mActor))
        al::setNerve(this, &AttackSignWait);
}

void SmallWanderBossStateAttack::exeAttackSignWait() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "AttackSignWait");
        al::startAction(mBullet, "AttackSignWait");
    }

    if (al::isGreaterEqualStep(this, 30))
        al::setNerve(this, &AttackStart);
}

void SmallWanderBossStateAttack::exeAttackStart() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "AttackStart");
        al::startAction(mBullet, "AttackStart");
    }

    if (al::isActionEnd(mActor))
        al::setNerve(this, &Attack);
}

void SmallWanderBossStateAttack::exeAttack() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "Attack");
        mBullet->startLaunch();
        mBullet = nullptr;
    }

    if (al::isActionEnd(mActor))
        al::setNerve(this, &AttackEnd);
}

void SmallWanderBossStateAttack::exeAttackEnd() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "AttackEnd");

    if (al::isActionEnd(mActor))
        kill();
}
