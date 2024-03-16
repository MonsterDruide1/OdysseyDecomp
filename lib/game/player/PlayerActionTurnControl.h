#pragma once

#include <math/seadVector.h>
#include "al/LiveActor/LiveActor.h"

class PlayerActionTurnControl {
public:
    PlayerActionTurnControl(al::LiveActor*);
    void setup(float, float, float, float, int, int, int);
    void reset();
    void update(const sead::Vector3f&, const sead::Vector3f&);
    void calcTilt(sead::Vector3f*, const sead::Vector3f&, float);
    float calcTurnPowerRate(const sead::Vector3f&) const;

private:
    const al::LiveActor* mLiveActor;
    float _8;
    sead::Vector3f _c;
    sead::Vector3f _18;
    sead::Vector3f _24;
    float _30;
    int _34;
    void* _38_array;
    int _40[2];
    int _48[2];
    sead::Vector3f _50;
    sead::Vector3f _5c;
    bool _68[4];
    float mTurnAngleStart;
    float mTurnAngleFast;
    float mTurnAngleLimit;
    float mTurnAngleFastLimit;
    int mTurnAccelFrame;
    int mTurnAccelFrameFast;
    int mTurnBrakeFrame;
    bool _88[2];
};
