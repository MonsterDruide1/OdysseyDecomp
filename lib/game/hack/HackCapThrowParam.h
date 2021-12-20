#pragma once

namespace al {
class LiveActor;
}

class HackCapThrowParam {
public:
    HackCapThrowParam(al::LiveActor* actor);

private:
    float* mHackThrowHeight;
    float* mMaxVel;
    float* mContinuousThrowSpeed;
    int* mBreakTime;
    float* mMaxDist;
    int* mEndpointStopTime;
    int* mMaxEndpointStopTime;
    float* mReturnStrength;
    float* mMaxRetSpeed;
    float* mTurnAngleLimit;
    float* mWaterMaxSpeed;
    float* mWaterDist;
    int* mWaterBreakTime;
    float* mWaterMaxRetSpeed;
    float* mTornadoDist;
    float* mTornadoMaxDist;
    int* mTornadoReflectTime;
    float* mRollSpeed;
    float* mRollDistTop;
    float* mRollDistBottom;
    int* mRollBrakeTimeTop;
    int* mRollBrakeTimeBottom;
    float* mRollGroundGroundedPoseTrack;
    float* mRollGroundAerialPoseTrack;
};
