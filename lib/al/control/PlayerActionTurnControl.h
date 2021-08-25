#pragma once

#include <math/seadVector.h>

namespace al{
    class LiveActor;
}

class PlayerActionTurnControl {

public:
    PlayerActionTurnControl(al::LiveActor*);
    void setup(float, float, float, float, int, int, int);
    void reset();
    void update(const sead::Vector3f&, const sead::Vector3f&);
    void calcTilt(sead::Vector3f*, const sead::Vector3f&, float);
    float calcTurnPowerRate(const sead::Vector3f&) const;

private:
    const al::LiveActor *mLiveActor;
    float val;
    sead::Vector3f someVector;
    sead::Vector3f someVectorTurnPowerRate;
    sead::Vector3f someVector2;
    float something;
    int placeholder;
    void *someArray;
    int someThings[2];
    int placeholder4[2];
    sead::Vector3f someVector4;
    sead::Vector3f someVector3;
    bool gap[4];
    float values[4];
    int values2[3];
    bool placeholder2[2];

};
