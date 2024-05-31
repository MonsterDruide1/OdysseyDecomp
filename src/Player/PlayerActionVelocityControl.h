#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
}
class IUsePlayerCollision;

class PlayerActionVelocityControl {
public:
    PlayerActionVelocityControl(al::LiveActor* actor, const IUsePlayerCollision* collider);

    void calcFrontBrake(f32 decel);
    void calcSideVelocityLimit(const sead::Vector3f& moveInput, f32 brakeSideAccel,
                               f32 brakeSideRate, f32 maxSideSpeed);
    void calcSideBrake(f32 decel);
    void calcTrample(f32 speedDown);
    void calcSnap(const sead::Vector3f& snapDirection, f32 snapDistance);
    void calcOnGround(const sead::Vector3f& groundNormal);
    void apply();

private:
    al::LiveActor* mActor;
    sead::Vector3f mSide = sead::Vector3f::zero;
    sead::Vector3f mUp = sead::Vector3f::zero;
    sead::Vector3f mVelocityFront = sead::Vector3f::zero;
    sead::Vector3f mVelocitySide = sead::Vector3f::zero;
    sead::Vector3f mVelocityUp = sead::Vector3f::zero;
};
