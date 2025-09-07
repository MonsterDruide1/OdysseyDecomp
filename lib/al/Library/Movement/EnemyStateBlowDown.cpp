#include "Library/Movement/EnemyStateBlowDown.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"

const al::EnemyStateBlowDownParam sEnemyStateBlowDownParam = al::EnemyStateBlowDownParam();

namespace al {
EnemyStateBlowDown::EnemyStateBlowDown(LiveActor* actor, const EnemyStateBlowDownParam* param,
                                       const char* name)
    : ActorStateBase(name, actor), mParam(param) {
    if (!mParam)
        mParam = &sEnemyStateBlowDownParam;
}

void EnemyStateBlowDown::start(const HitSensor* other) {
    sead::Vector3f dir = getSensorPos(other) - getTrans(mActor);

    verticalizeVec(&dir, getGravity(mActor), dir);

    if (!tryNormalizeOrZero(&dir))
        calcFrontDir(&dir, mActor);

    start(-dir);
}

void EnemyStateBlowDown::start(const sead::Vector3f& dir) {
    if (mParam->faceAwayFromActor)
        faceToDirection(mActor, -dir);

    auto* actor = mActor;
    sead::Vector3f direction = dir * mParam->velocityStrength;
    sead::Vector3f gravity = getGravity(actor);
    sead::Vector3f velocity = gravity * mParam->gravityStrength;
    setVelocity(actor, direction - velocity);
}

void EnemyStateBlowDown::start(const HitSensor* other, const HitSensor* self) {
    sead::Vector3f dir = getSensorPos(other) - getSensorPos(self);

    verticalizeVec(&dir, getGravity(mActor), dir);

    if (!tryNormalizeOrZero(&dir))
        calcFrontDir(&dir, mActor);

    start(-dir);
}

void EnemyStateBlowDown::start(const LiveActor* attacker) {
    sead::Vector3f dir;
    calcFrontDir(&dir, attacker);

    start(-dir);
}

void EnemyStateBlowDown::appear() {
    NerveStateBase::appear();
    if (isInvalidClipping(mActor))
        mIsInvalidClipping = true;
    else {
        invalidateClipping(mActor);
        mIsInvalidClipping = false;
    }
    mBlowDownTimer = 0;
}

void EnemyStateBlowDown::kill() {
    NerveStateBase::kill();
    if (!mIsInvalidClipping)
        validateClipping(mActor);
}

void EnemyStateBlowDown::control() {
    if (mBlowDownTimer == 0) {
        if (mParam->actionName)
            startAction(mActor, mParam->actionName);
        if (isExistActorCollider(mActor))
            onCollide(mActor);
    }
    if ((mParam->blowDownLength <= mBlowDownTimer) ||
        (mParam->actionName && isActionOneTime(mActor, mParam->actionName) &&
         isActionEnd(mActor)) ||
        (isExistActorCollider(mActor) && isOnGround(mActor, 0))) {
        kill();
        return;
    }
    addVelocityToGravity(mActor, mParam->velocityMultiplier);
    scaleVelocity(mActor, mParam->velocityScale);
    mBlowDownTimer++;
}

EnemyStateBlowDownParam::EnemyStateBlowDownParam() = default;

EnemyStateBlowDownParam::EnemyStateBlowDownParam(const char* actionName) : actionName(actionName) {}

EnemyStateBlowDownParam::EnemyStateBlowDownParam(const char* actionName, f32 velocityStrength,
                                                 f32 gravityStrength, f32 velocityMultiplier,
                                                 f32 velocityScale, s32 blowDownLength,
                                                 bool isFaceAwayFromActor)
    : actionName(actionName), velocityStrength(velocityStrength), gravityStrength(gravityStrength),
      velocityMultiplier(velocityMultiplier), velocityScale(velocityScale),
      blowDownLength(blowDownLength), isFaceAwayFromActor(isFaceAwayFromActor) {}
}  // namespace al
