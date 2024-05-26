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
    f32 calcTurnPowerRate(const sead::Vector3f&) const;

public:
    al::LiveActor* mActor;
    float _8 = 0;
    sead::Vector3f _c = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _18 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _24 = {0.0f, 0.0f, 0.0f};
    float _30 = 0;
    int _34 = 0;
    float* arr = 0;
    int arr_size = 0;
    int _44 = 0;
    int _48 = 0;
    int _4c = 0;
    sead::Vector3f _50 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _5c = {0.0f, 0.0f, 0.0f};
    bool _68 = 0;
    bool _69 = 0;
    bool _6a = 0;
    float mTurnAngleStart = 0;
    float mTurnAngleFast = 0;
    float mTurnAngleLimit = 0;
    float mTurnAngleFastLimit = 0;
    int mTurnAccelFrame = 0;
    int mTurnAccelFrameFast = 0;
    int mTurnBrakeFrame = 0;
    bool _88 = 0;
    bool _89 = 0;
};
