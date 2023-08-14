#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}

class HackCapThrowParam {
public:
    HackCapThrowParam(al::LiveActor* actor);

private:
    f32* mHackThrowHeight;
    f32* mMaxVel;
    f32* mContinuousThrowSpeed;
    s32* mBreakTime;
    f32* mMaxDist;
    s32* mEndpointStopTime;
    s32* mMaxEndpointStopTime;
    f32* mReturnStrength;
    f32* mMaxRetSpeed;
    f32* mTurnAngleLimit;
    f32* mWaterMaxSpeed;
    f32* mWaterDist;
    s32* mWaterBreakTime;
    f32* mWaterMaxRetSpeed;
    f32* mTornadoDist;
    f32* mTornadoMaxDist;
    s32* mTornadoReflectTime;
    f32* mRollSpeed;
    f32* mRollDistTop;
    f32* mRollDistBottom;
    s32* mRollBrakeTimeTop;
    s32* mRollBrakeTimeBottom;
    f32* mRollGroundGroundedPoseTrack;
    f32* mRollGroundAerialPoseTrack;
};
