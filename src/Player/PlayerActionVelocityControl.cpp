#include "Player/PlayerActionVelocityControl.h"

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"

#include "Player/PlayerActionFunction.h"
#include "Util/PlayerCollisionUtil.h"

PlayerActionVelocityControl::PlayerActionVelocityControl(al::LiveActor* actor,
                                                         const IUsePlayerCollision* collision)
    : mActor(actor) {
    sead::Vector3f negGravity = -al::getGravity(actor);
    if (collision)
        rs::calcGroundNormalOrGravityDir(&negGravity, actor, collision);

    sead::Vector3f up;
    al::calcUpDir(&up, mActor);
    al::normalize(&up);

    sead::Vector3f front;
    al::calcFrontDir(&front, mActor);
    al::alongVectorNormalH(&front, front, up, negGravity);
    al::normalize(&front);

    mUp = negGravity;
    mSide.setCross(negGravity, front);
    al::normalize(&mSide);

    al::parallelizeVec(&mVelocityFront, front, al::getVelocity(actor));
    al::parallelizeVec(&mVelocitySide, mSide, al::getVelocity(actor));
    mVelocityUp = al::getVelocity(actor) - mVelocityFront - mVelocitySide;
}

void PlayerActionVelocityControl::calcFrontBrake(f32 decel) {
    mVelocityFront *= decel;
}

void PlayerActionVelocityControl::calcSideVelocityLimit(const sead::Vector3f& moveInput,
                                                        f32 brakeSideAccel, f32 brakeSideRate,
                                                        f32 maxSideSpeed) {
    f32 stickPow = PlayerActionFunction::calcStickPow(moveInput.dot(mSide));
    if (al::isNearZero(stickPow, 0.01f)) {
        mVelocitySide *= brakeSideRate;
        return;
    }

    mVelocitySide += (stickPow * brakeSideAccel) * mSide;
    al::limitLength(&mVelocitySide, mVelocitySide, maxSideSpeed);
}

void PlayerActionVelocityControl::calcSideBrake(f32 decel) {
    mVelocitySide *= decel;
}

void PlayerActionVelocityControl::calcTrample(f32 downVel) {
    al::verticalizeVec(&mVelocityUp, al::getGravity(mActor), mVelocityUp);
    mVelocityUp += al::getGravity(mActor) * downVel;
}

void PlayerActionVelocityControl::calcSnap(const sead::Vector3f& snapDir, f32 snapDistance) {
    // requires to be this explicit to match. Both += and vector operations mismatch.
    mVelocityUp.x = (snapDir.x * snapDistance) + mVelocityUp.x;
    mVelocityUp.y = (snapDir.y * snapDistance) + mVelocityUp.y;
    mVelocityUp.z = (snapDir.z * snapDistance) + mVelocityUp.z;
}

void PlayerActionVelocityControl::calcOnGround(const sead::Vector3f& groundNormal) {
    al::verticalizeVec(&mVelocityFront, groundNormal, mVelocityFront);
    al::verticalizeVec(&mVelocitySide, groundNormal, mVelocitySide);
    al::verticalizeVec(&mVelocityUp, groundNormal, mVelocityUp);
}

void PlayerActionVelocityControl::apply() {
    *al::getVelocityPtr(mActor) = mVelocityFront + mVelocitySide + mVelocityUp;
}
