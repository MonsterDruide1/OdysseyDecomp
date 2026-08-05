#include "Boss/Koopa/KoopaStateDeadAndDemoBattleEnd.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Boss/BossUtil/BossUtil.h"
#include "Boss/Koopa/KoopaCap.h"
#include "Boss/Koopa/KoopaDemoExecutor.h"
#include "Boss/Koopa/KoopaFunction.h"
#include "Boss/Koopa/KoopaWeaponHolder.h"
#include "Util/DemoUtil.h"

namespace {
IUseDemoSkip* getDemoSkip(KoopaStateDeadAndDemoBattleEnd* state) {
    return state;
}

NERVE_IMPL(KoopaStateDeadAndDemoBattleEnd, Dead);
NERVE_IMPL(KoopaStateDeadAndDemoBattleEnd, Demo);
NERVE_IMPL(KoopaStateDeadAndDemoBattleEnd, Skip);
NERVE_IMPL(KoopaStateDeadAndDemoBattleEnd, Start);
NERVES_MAKE_NOSTRUCT(KoopaStateDeadAndDemoBattleEnd, Dead, Demo, Skip, Start);
}  // namespace

KoopaStateDeadAndDemoBattleEnd::KoopaStateDeadAndDemoBattleEnd(
    al::LiveActor* actor, KoopaDemoExecutor* demoExecutor, KoopaCap* cap,
    KoopaWeaponHolder* weaponHolder, al::LiveActor* demoActor, bool isKoopaLv2)
    : al::ActorStateBase("死亡~戦闘終了デモ", actor), mDemoExecutor(demoExecutor), mCap(cap),
      mWeaponHolder(weaponHolder), mDemoActor(demoActor), mIsKoopaLv2(isKoopaLv2) {
    initNerve(&Dead, 0);
}

void KoopaStateDeadAndDemoBattleEnd::appear() {
    NerveStateBase::appear();
    al::setNerve(this, &Dead);
    al::invalidateHitSensors(mActor);
}

void KoopaStateDeadAndDemoBattleEnd::kill() {
    NerveStateBase::kill();
    rs::requestEndDemoWithPlayer(mActor);
    if (mIsKoopaLv2)
        rs::saveShowDemoBossBattleEndKoopaLv2(mActor);
    mDemoActor->makeActorDead();
}

bool KoopaStateDeadAndDemoBattleEnd::isFirstDemo() const {
    if (mIsKoopaLv2)
        return rs::isAlreadyShowDemoBossBattleEndKoopaLv2(mActor) ^ 1;
    return true;
}

bool KoopaStateDeadAndDemoBattleEnd::isEnableSkipDemo() const {
    if (al::isNerve(this, &Demo))
        return al::isFirstStep(this) ^ 1;
    return false;
}

void KoopaStateDeadAndDemoBattleEnd::skipDemo() {
    mDemoExecutor->skip();
    KoopaFunction::onSwitchGraphicsLevelBattleEnd(mActor);
    al::setNerve(this, &Skip);
    kill();
}

void KoopaStateDeadAndDemoBattleEnd::exeDead() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "Die");
        al::setVelocityZero(mActor);
    }

    if (al::isStep(this, 1))
        al::startHitReaction(mActor, "ダメージとどめ");

    if (al::isActionEnd(mActor)) {
        KoopaStateDeadAndDemoBattleEnd* demoSkip = this;
        if (mDemoExecutor->tryStartBattleEndDemo(demoSkip)) {
            al::setNerve(this, &Demo);
            return;
        }
        al::setNerve(this, &Start);
    }
}

void KoopaStateDeadAndDemoBattleEnd::exeStart() {
    if (mDemoExecutor->tryStartBattleEndDemo(getDemoSkip(this)))
        al::setNerve(this, &Demo);
}

void KoopaStateDeadAndDemoBattleEnd::exeDemo() {
    if (al::isFirstStep(this)) {
        mCap->endEquipAndKill();
        mWeaponHolder->makeActorDeadAll();
        KoopaFunction::offSwitchBattleKeepOn(mActor);
    }

    if (al::isGreaterEqualStep(this, 436))
        KoopaFunction::onSwitchGraphicsLevelBattleEnd(mActor);

    if (mDemoExecutor->update())
        kill();
}

void KoopaStateDeadAndDemoBattleEnd::exeSkip() {}
