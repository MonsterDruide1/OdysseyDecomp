#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
}
class PlayerConst;
class PlayerInput;
class IUsePlayerCollision;

class PlayerActionSlopeSlideControl {
public:
    PlayerActionSlopeSlideControl(al::LiveActor*, const PlayerConst*, const PlayerInput*,
                                  const IUsePlayerCollision*);
    void setup();
    void setupCutSlideOppositeDir();
    f32 update(f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, bool);

    const sead::Vector3f& getDirSlide() const { return mDirSlide; }

    const sead::Vector3f& getGroundNormal() const { return mGroundNormal; }

    const sead::Vector3f& getHorizontalVelocity() const { return mHorizontalVelocity; }

private:
    al::LiveActor* mPlayer;
    const PlayerConst* mConst;
    const PlayerInput* mInput;
    const IUsePlayerCollision* mCollision;
    sead::Vector3f mDirSlide = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mGroundNormal = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mHorizontalVelocity = {0.0f, 0.0f, 0.0f};
};

static_assert(sizeof(PlayerActionSlopeSlideControl) == 0x48);
