#include "Library/Movement/EnemyStateBlowDown.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Movement/EnemyStateBlowDownParam.h"

namespace al {
void EnemyStateBlowDown::start(const HitSensor* sensor) {
    sead::Vector3f dir = getSensorPos(sensor) - getTrans(mActor);

    verticalizeVec(&dir, getGravity(mActor), dir);

    if (!tryNormalizeOrZero(&dir))
        calcFrontDir(&dir, mActor);

    start(-dir);
}

void EnemyStateBlowDown::start(const sead::Vector3f& dir) {
    if (mParam->mFaceAwayFromActor) {
        faceToDirection(mActor, -dir);
    }

    auto* actor = mActor;
    sead::Vector3f direction = dir * mParam->mGravityStrength;
    sead::Vector3f gravity = getGravity(actor);
    sead::Vector3f velocity = gravity * mParam->mVelocityStrength;
    setVelocity(actor, direction - velocity);
}

void EnemyStateBlowDown::start(const HitSensor* sensor1, const HitSensor* sensor2) {
    sead::Vector3f dir = getSensorPos(sensor1) - getSensorPos(sensor2);

    verticalizeVec(&dir, getGravity(mActor), dir);

    if (!tryNormalizeOrZero(&dir))
        calcFrontDir(&dir, mActor);

    start(-dir);
}

void EnemyStateBlowDown::start(const LiveActor* actor) {
    sead::Vector3f dir;
    calcFrontDir(&dir, actor);

    start(-dir);
}

void EnemyStateBlowDown::appear() {
    setDead(false);
    if (isInvalidClipping(mActor))
        mIsInvalidClipping = true;
    else {
        invalidateClipping(mActor);
        mIsInvalidClipping = false;
    }
    mBlowDownTimer = 0;
}

void EnemyStateBlowDown::kill() {
    setDead(true);
    if (!mIsInvalidClipping)
        validateClipping(mActor);
}

void EnemyStateBlowDown::control() {
    if (mBlowDownTimer == 0) {
        if (mParam->mActionName)
            startAction(mActor, mParam->mActionName);
        if (isExistActorCollider(mActor))
            onCollide(mActor);
    }
    if ((mParam->mBlowDownLength <= mBlowDownTimer) ||
        (mParam->mActionName && isActionOneTime(mActor, mParam->mActionName) &&
         isActionEnd(mActor)) ||
        (isExistActorCollider(mActor) && isOnGround(mActor, 0))) {
        kill();
        return;
    }
    addVelocityToGravity(mActor, mParam->mVelocityMultiplier);
    scaleVelocity(mActor, mParam->mVelocityScale);
    mBlowDownTimer++;
}
}  // namespace al
