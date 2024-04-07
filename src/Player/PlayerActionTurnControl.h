#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
}

class PlayerActionTurnControl {
public:
    PlayerActionTurnControl(al::LiveActor*);
    void setup(float turnAngleStart, float turnAngleFast, float turnAngleLimit,
               float turnAngleFastLimit, int turnAccelFrame, int turnAccelFrameFast,
               int turnBrakeFrame);
    void reset();
    void update(const sead::Vector3f&, const sead::Vector3f&);
    void calcTilt(sead::Vector3f*, const sead::Vector3f&, float);
    float calcTurnPowerRate(const sead::Vector3f&) const;

private:
    const al::LiveActor* mLiveActor;
    float _8 = 0;
    sead::Vector3f _c = {0, 0, 0};
    sead::Vector3f _18 = {0, 0, 0};
    sead::Vector3f _24 = {0, 0, 0};
    float _30 = 0;
    int _34 = 0;
    float* mArr = nullptr;
    int mArrSize = 0;
    int _44 = 0;
    int _48 = 0;
    int _4c = 0;
    sead::Vector3f _50 = {0, 0, 0};
    sead::Vector3f _5c = {0, 0, 0};
    bool _68 = false;
    bool _69 = false;
    bool _6a = false;
    bool pad;
    float mTurnAngleStart = 0;
    float mTurnAngleFast = 0;
    float mTurnAngleLimit = 0;
    float mTurnAngleFastLimit = 0;
    int mTurnAccelFrame = 0;
    int mTurnAccelFrameFast = 0;
    int mTurnBrakeFrame = 0;
    bool _88 = false;
    bool _89 = false;
    bool pad2[6];
};
