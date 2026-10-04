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
    initNerve(&AttackSign);
}

void GiantWanderBossStateAttack::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &AttackSign);
}

void GiantWanderBossStateAttack::kill() {
    al::NerveStateBase::kill();

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
    mMineAttackType = MineAttackType::FirstPhase;
    al::setNerve(this, &AttackSign);
}

void GiantWanderBossStateAttack::startWithMineEscape(GiantWanderBossMine* mine) {
    mBullet = nullptr;
    mMine = mine;
    mMineAttackType = MineAttackType::Escape;
    al::setNerve(this, &AttackSign);
}

void GiantWanderBossStateAttack::startWithMineLongRange(GiantWanderBossMine* mine) {
    mBullet = nullptr;
    mMine = mine;
    mMineAttackType = MineAttackType::LongRange;
    al::setNerve(this, &AttackSign);
}

void GiantWanderBossStateAttack::exeAttackSign() {
    if (al::isFirstStep(this))
        startAttackAction("AttackSign");

    if (al::isActionEnd(mActor))
        al::setNerve(this, &AttackSignWait);
}

void GiantWanderBossStateAttack::exeAttackSignWait() {
    if (al::isFirstStep(this))
        startAttackAction("AttackSignWait");

    if (al::isGreaterEqualStep(this, 30))
        al::setNerve(this, &AttackStart);
}

void GiantWanderBossStateAttack::exeAttackStart() {
    if (al::isFirstStep(this))
        startAttackAction("AttackStart");

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
            switch (mMineAttackType) {
            case MineAttackType::LongRange:
                mMine->startLaunchForLongRange();
                break;
            case MineAttackType::FirstPhase:
                mMine->startLaunchForFirstPhase();
                break;
            case MineAttackType::Escape:
                mMine->startLaunchForEscape();
                break;
            default:
                break;
            }

            mMine = nullptr;
        }
    }

    if (al::isActionEnd(mActor))
        al::setNerve(this, &AttackEnd);
}

void GiantWanderBossStateAttack::exeAttackEnd() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "AttackEnd");

    if (al::isActionEnd(mActor))
        kill();
}

inline void GiantWanderBossStateAttack::startAttackAction(const char* actionName) {
    al::startAction(mActor, actionName);

    if (mMine)
        al::startAction(mMine, actionName);

    if (mBullet)
        al::startAction(mBullet, actionName);
}
