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
    PlayerActionSlopeSlideControl(al::LiveActor*, PlayerConst const*, PlayerInput const*,
                                  IUsePlayerCollision const*);
    void setup();
    void setupCutSlideOppositeDir();
    f32 update(float, float, float, float, float, float, float, float, float, float, bool);

public:
    al::LiveActor* mPlayer;
    const PlayerConst* mConst;
    const PlayerInput* mInput;
    const IUsePlayerCollision* mCollision;
    sead::Vector3f _20 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _2c = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _38 = {0.0f, 0.0f, 0.0f};
};
