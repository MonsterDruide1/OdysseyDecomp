#include "Boss/BossUtil/BossStateChasePlayer.h"

#include <cmath>

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"

namespace {
NERVE_IMPL(BossStateChasePlayer, Chase)
NERVE_IMPL(BossStateChasePlayer, Stop)
NERVE_IMPL(BossStateChasePlayer, Lost)

NERVES_MAKE_NOSTRUCT(BossStateChasePlayer, Chase, Stop, Lost)

const BossStateChasePlayerParam sDefaultParam = {1.0f, 0.3f, 0.9f,  500.0f, 0.2f,
                                                 2.0f, 0.1f, 10.0f, 90.0f};
}  // namespace

BossStateChasePlayer::BossStateChasePlayer(const char* name, al::LiveActor* actor,
                                           const BossStateChasePlayerParam* param)
    : al::ActorStateBase(name, actor), mParam(param) {
    initNerve(&Chase, 0);
    if (!param)
        mParam = &sDefaultParam;
}

void BossStateChasePlayer::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &Chase);
}

void BossStateChasePlayer::startStop() {
    al::setNerve(this, &Stop);
}

// NON_MATCHING: https://decomp.me/scratch/pvWjE/ - Stack frame and register allocation differ
// (target uses d8-d11/x20-x21 as callee-saved regs, extra loads of mRotationAngle/mTargetPlayer
// early); dirToPlayer built on-stack with different field ordering; subtraction operand order
// flipped in several fsub instructions
void BossStateChasePlayer::exeChase() {
    mTargetPlayer = al::tryFindAlivePlayerActorFirst(mActor);
    if (!mTargetPlayer) {
        al::setNerve(this, &Lost);
        return;
    }

    // Compute direction from actor to player (horizontal only, for rotation)
    sead::Vector3f dirToPlayer;
    dirToPlayer = al::getTrans(mTargetPlayer);
    const sead::Vector3f& actorTrans = al::getTrans(mActor);
    dirToPlayer.x -= actorTrans.x;
    dirToPlayer.y = 0.0f;
    dirToPlayer.z -= actorTrans.z;

    // Update rotation toward player
    if (al::tryNormalizeOrZero(&dirToPlayer)) {
        sead::Vector3f frontDir = sead::Vector3f::ez;
        al::calcFrontDir(&frontDir, mActor);

        f32 angle = al::calcAngleOnPlaneDegree(frontDir, dirToPlayer, sead::Vector3f::ey);
        f32 sign = al::sign(angle);

        // Choose turn speed based on whether we're already roughly facing the player
        f32 turnSpeed;
        if (angle <= -mParam->frontAngleThreshold || angle >= mParam->frontAngleThreshold)
            turnSpeed = mParam->turnSpeedFast;
        else
            turnSpeed = mParam->turnSpeedSlow;

        // Clamp rotation to turn speed, preserving sign
        f32 absAngle = angle > 0.0f ? angle : -angle;
        f32 absTurnSpeed = turnSpeed > 0.0f ? turnSpeed : -turnSpeed;
        f32 targetRotation = sign * (absAngle < absTurnSpeed ? absAngle : absTurnSpeed);

        mRotationAngle = al::converge(mRotationAngle, targetRotation, mParam->turnConvergeSpeed);
        al::rotateQuatYDirDegree(mActor, mRotationAngle);
    } else {
        mRotationAngle = al::converge(mRotationAngle, 0.0f, mParam->turnConvergeSpeed);
    }

    // Compute current XZ speed
    const sead::Vector3f& vel = al::getVelocity(mActor);
    f32 speedXZ = sqrtf(vel.x * vel.x + vel.z * vel.z);

    // Get fresh front direction after rotation
    sead::Vector3f frontDir = sead::Vector3f::ez;
    al::calcFrontDir(&frontDir, mActor);

    // Recompute direction to player for speed logic
    dirToPlayer = al::getTrans(mTargetPlayer);
    const sead::Vector3f& actorTrans2 = al::getTrans(mActor);
    dirToPlayer.x -= actorTrans2.x;
    dirToPlayer.y = 0.0f;
    dirToPlayer.z -= actorTrans2.z;

    bool decelerate = false;
    if (al::tryNormalizeOrZero(&dirToPlayer)) {
        if (al::calcAngleDegree(frontDir, dirToPlayer) > mParam->chaseAngleMax)
            decelerate = true;
    }

    f32 newSpeed;
    if (!decelerate) {
        // Check if close enough to stop accelerating
        const sead::Vector3f& at = al::getTrans(mActor);
        const sead::Vector3f& pt = al::getTrans(mTargetPlayer);
        f32 dx = at.x - pt.x;
        f32 dz = at.z - pt.z;
        f32 dist = sqrtf(dx * dx + dz * dz);
        if (dist <= mParam->proximityStopDist)
            decelerate = true;
    }

    if (decelerate)
        newSpeed = al::converge(speedXZ, 0.0f, mParam->deceleration);
    else
        newSpeed = speedXZ + mParam->acceleration;

    al::setVelocityToDirection(mActor, frontDir, newSpeed);
    al::scaleVelocityDirection(mActor, frontDir, mParam->scaleVelocity);
}

void BossStateChasePlayer::exeLost() {
    const sead::Vector3f& vel = al::getVelocity(mActor);
    f32 speed = sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);
    f32 newSpeed = al::converge(speed, 0.0f, mParam->deceleration);

    sead::Vector3f newVelocity = al::getVelocity(mActor);
    f32 len = sqrtf(newVelocity.x * newVelocity.x + newVelocity.y * newVelocity.y +
                    newVelocity.z * newVelocity.z);
    if (len > 0.0f) {
        f32 scale = newSpeed / len;
        newVelocity.x *= scale;
        newVelocity.y *= scale;
        newVelocity.z *= scale;
    }
    al::setVelocity(mActor, newVelocity);

    mTargetPlayer = al::tryFindAlivePlayerActorFirst(mActor);
    if (mTargetPlayer)
        al::setNerve(this, &Chase);
}

void BossStateChasePlayer::exeStop() {
    const sead::Vector3f& vel = al::getVelocity(mActor);
    f32 speedXZ = sqrtf(vel.x * vel.x + vel.z * vel.z);
    f32 newSpeed = al::converge(speedXZ, 0.0f, mParam->deceleration);

    al::setVelocityToDirection(mActor, al::getVelocity(mActor), newSpeed);
    if (al::isNearZero(newSpeed, 0.001f))
        kill();
}
