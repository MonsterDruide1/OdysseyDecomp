#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
}

class PlayerActionTurnControl {
public:
    PlayerActionTurnControl(al::LiveActor*);
    void setup(f32, f32, f32, f32, s32, s32, s32);
    void reset();
    void update(const sead::Vector3f&, const sead::Vector3f&);
    void calcTilt(sead::Vector3f*, const sead::Vector3f&, f32);
    void calcTurnPowerRate(const sead::Vector3f&) const;

private:
    void* filler[0x90 / 8];
};

static_assert(sizeof(PlayerActionTurnControl) == 0x90);
