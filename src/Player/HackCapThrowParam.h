#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ActorParamF32;
struct ActorParamS32;
class LiveActor;
}  // namespace al

class HackCapThrowParam {
public:
    HackCapThrowParam(al::LiveActor* actor);

private:
    al::ActorParamF32* mHackThrowHeight;
    al::ActorParamF32* mMaxVel;
    al::ActorParamF32* mContinuousThrowSpeed;
    al::ActorParamS32* mBreakTime;
    al::ActorParamF32* mMaxDist;
    al::ActorParamS32* mEndpointStopTime;
    al::ActorParamS32* mMaxEndpointStopTime;
    al::ActorParamF32* mReturnStrength;
    al::ActorParamF32* mMaxRetSpeed;
    al::ActorParamF32* mTurnAngleLimit;
    al::ActorParamF32* mWaterMaxSpeed;
    al::ActorParamF32* mWaterDist;
    al::ActorParamS32* mWaterBreakTime;
    al::ActorParamF32* mWaterMaxRetSpeed;
    al::ActorParamF32* mTornadoDist;
    al::ActorParamF32* mTornadoMaxDist;
    al::ActorParamS32* mTornadoReflectTime;
    al::ActorParamF32* mRollSpeed;
    al::ActorParamF32* mRollDistTop;
    al::ActorParamF32* mRollDistBottom;
    al::ActorParamS32* mRollBrakeTimeTop;
    al::ActorParamS32* mRollBrakeTimeBottom;
    al::ActorParamF32* mRollGroundGroundedPoseTrack;
    al::ActorParamF32* mRollGroundAerialPoseTrack;
};
