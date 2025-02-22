#include "Enemy/KuromadoMagicBall.h"

#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
<<<<<<< HEAD
#include "Library/LiveActor/ActorPoseUtil.h"
=======
#include "Library/LiveActor/ActorPoseKeeper.h"
<<<<<<< HEAD
#include "Library/LiveActor/ActorSensorMsgFunction.h"
>>>>>>> 9ae314e (PR: fix clang-format for headers with changed include)
=======
>>>>>>> b6e1615 (Merge ActorSensorMsgFunction into ActorSensorUtil)
#include "Library/LiveActor/ActorSensorUtil.h"

#include "Util/SensorMsgFunction.h"

KuromadoMagicBall::KuromadoMagicBall(const char* actorName) : al::LiveActor(actorName) {}

void KuromadoMagicBall::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initActorPoseTFSV(this);
    al::initActorSRT(this, info);
    al::initActorClipping(this, info);
    al::invalidateClipping(this);
    al::initActorEffectKeeper(this, info, "KuromadoMagicBall");
    al::initExecutorEnemyMovement(this, info);
    initHitSensor(1);
    al::addHitSensorEnemyAttack(this, info, "Attack", 120.0f, 8, sead::Vector3f(0.0f, 0.0f, 0.0f));
    makeActorDead();
}

void KuromadoMagicBall::appear() {
    al::LiveActor::appear();
    al::emitEffect(this, "Core", nullptr);
    al::setVelocity(this, al::getFront(this) * 20.0f);
    mAliveTimer = 0;
}

void KuromadoMagicBall::control() {
    mAliveTimer += 1;
    if (mAliveTimer >= 300) {
        al::emitEffect(this, "Disappear", nullptr);
        kill();
    }
}

void KuromadoMagicBall::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::sendMsgEnemyAttackFire(other, self, nullptr) ||
        rs::sendMsgEnemyAttackStrong(other, self) || rs::sendMsgEnemyAttackDash(other, self)) {
        al::emitEffect(this, "Hit", nullptr);
        kill();
    }
}
