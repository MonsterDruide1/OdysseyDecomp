#include "Enemy/EnemyStateDamageCap.h"
#include "Enemy/EnemyCap.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Util/Sensor.h"

namespace {
NERVE_IMPL(EnemyStateDamageCap, Wait);
NERVE_IMPL(EnemyStateDamageCap, DamageCap);

struct {
    NERVE_MAKE(EnemyStateDamageCap, Wait);
    NERVE_MAKE(EnemyStateDamageCap, DamageCap);
} NrvEnemyStateDamageCap;

}  // namespace

EnemyStateDamageCap::EnemyStateDamageCap(al::LiveActor* actor)
    : al::ActorStateBase("キャプチャ敵の帽子反応ステート", actor) {
    initNerve(&NrvEnemyStateDamageCap.Wait, 0);
}

void EnemyStateDamageCap::kill() {
    al::setNerve(this, &NrvEnemyStateDamageCap.Wait);
    setDead(true);
}

void EnemyStateDamageCap::createEnemyCap(const al::ActorInitInfo& info, const char* name) {
    mEnemyCap = rs::tryCreateEnemyCap(mActor, info, name);
}

bool EnemyStateDamageCap::tryReceiveMsgCapBlow(const al::SensorMsg* msg, al::HitSensor* source,
                                               al::HitSensor* target) {
    if (!isCapOn() || !rs::isMsgCapAttack(msg))
        return false;
    rs::requestHitReactionToAttacker(msg, target, source);
    mEnemyCap->startBlowDown(source);
    al::setNerve(this, &NrvEnemyStateDamageCap.Wait);
    return true;
}

bool EnemyStateDamageCap::isCapOn() const {
    return mEnemyCap && al::isAlive(mEnemyCap) && !mEnemyCap->isBlowDown();
}

void EnemyStateDamageCap::blowCap(al::HitSensor* source) {
    if (isCapOn())
        mEnemyCap->startBlowDown(source);
}

void EnemyStateDamageCap::resetCap() {
    if (mEnemyCap)
        mEnemyCap->appear();
}

void EnemyStateDamageCap::makeActorDeadCap() {
    if (mEnemyCap)
        mEnemyCap->makeActorDead();
}

void EnemyStateDamageCap::exeWait() {}

void EnemyStateDamageCap::exeDamageCap() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "DamageCap");
    if (al::isActionEnd(mActor))
        kill();
}