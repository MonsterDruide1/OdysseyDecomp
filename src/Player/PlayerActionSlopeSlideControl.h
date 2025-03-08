#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
}
class PlayerConst;
class PlayerInput;
class IUsePlayerCollision;

class PlayerActionSlopeSlideControl {
public:
    PlayerActionSlopeSlideControl(al::LiveActor* player, const PlayerConst* pConst,
                                  const PlayerInput* input, const IUsePlayerCollision* collider);
    void setup();
    void setupCutSlideOppositeDir();
    f32 update(f32 accel, f32 brake, f32 against, f32 anglePowerMax, f32 maxSpeed, f32 sideAccel,
               f32 sideBrake, f32 sideMaxSpeed, f32 turnDegree, f32 gravity, bool isRolling);

    const sead::Vector3f& getDirSlide() const { return mDirSlide; }

    const sead::Vector3f& getGroundNormal() const { return mGroundNormal; }

    const sead::Vector3f& getHorizontalVelocity() const { return mHorizontalVelocity; }

private:
    al::LiveActor* mPlayer;
    const PlayerConst* mConst;
    const PlayerInput* mInput;
    const IUsePlayerCollision* mCollision;
    sead::Vector3f mDirSlide = sead::Vector3f::zero;
    sead::Vector3f mGroundNormal = sead::Vector3f::zero;
    sead::Vector3f mHorizontalVelocity = sead::Vector3f::zero;
};

static_assert(sizeof(PlayerActionSlopeSlideControl) == 0x48);
