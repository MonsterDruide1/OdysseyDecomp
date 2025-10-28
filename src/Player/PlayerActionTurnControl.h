#pragma once

#include <container/seadRingBuffer.h>
#include <math/seadVector.h>
#include "container/seadRingBuffer.h"

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
    f32 calcTurnPowerRate(const sead::Vector3f&) const;

    void set_88(bool value) { _88 = value; }

    const sead::Vector3f& get_5c() const { return _5c; }

    bool get_69() const { return _69; }

public:
    al::LiveActor* mPlayer;  // mActor
    f32 _8 = 0.0f;
    sead::Vector3f _c = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _18 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _24 = {0.0f, 0.0f, 0.0f};
    f32 _30 = 0.0f;
    sead::RingBuffer<f32> _38;  // arr/arr_size/_44/_48/_4c
    sead::Vector3f _50 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _5c = {0.0f, 0.0f, 0.0f};
    bool _68 = false;
    bool _69 = false;
    bool _6a = false;
    f32 mTurnAngleStart = 0.0f;
    f32 mTurnAngleFast = 0.0f;
    f32 mTurnAngleLimit = 0.0f;
    f32 mTurnAngleFastLimit = 0.0f;
    s32 mTurnAccelFrame = 0;
    s32 mTurnAccelFrameFast = 0;
    s32 mTurnBrakeFrame = 0;
    bool _88 = false;
    bool _89 = false;
};

static_assert(sizeof(PlayerActionTurnControl) == 0x90);
