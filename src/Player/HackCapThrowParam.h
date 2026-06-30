#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/ActorParamHolderUtil.h"

namespace al {
struct ActorParamF32;
struct ActorParamS32;
class LiveActor;
}  // namespace al

class HackCapThrowParam {
public:
    HackCapThrowParam(al::LiveActor* actor) {
        mHackThrowHeight = al::findActorParamF32(actor, "投げる高さ");
        mMaxVel = al::findActorParamF32(actor, "最高速度");
        mContinuousThrowSpeed = al::findActorParamF32(actor, "連続投げ速度");
        mBreakTime = al::findActorParamS32(actor, "ブレーキ時間");
        mMaxDist = al::findActorParamF32(actor, "到達距離");
        mEndpointStopTime = al::findActorParamS32(actor, "端点停止時間");
        mMaxEndpointStopTime = al::findActorParamS32(actor, "最大端点停止時間");
        mReturnStrength = al::findActorParamF32(actor, "戻り強さ");
        mMaxRetSpeed = al::findActorParamF32(actor, "戻り最高速度");
        mTurnAngleLimit = al::findActorParamF32(actor, "ターン限界角度");
        mWaterMaxSpeed = al::findActorParamF32(actor, "[水中]最高速度");
        mWaterDist = al::findActorParamF32(actor, "[水中]到達距離");
        mWaterBreakTime = al::findActorParamS32(actor, "[水中]ブレーキ時間");
        mWaterMaxRetSpeed = al::findActorParamF32(actor, "[水中]戻り最高速度");
        mTornadoDist = al::findActorParamF32(actor, "[竜巻投げ]到達距離");
        mTornadoMaxDist = al::findActorParamF32(actor, "[竜巻投げ]最高到達距離");
        mTornadoReflectTime = al::findActorParamS32(actor, "[竜巻投げ]反射時間");
        mRollSpeed = al::findActorParamF32(actor, "[転がし投げ]速度");
        mRollDistTop = al::findActorParamF32(actor, "[転がし投げ]到達距離[上]");
        mRollDistBottom = al::findActorParamF32(actor, "[転がし投げ]到達距離[下]");
        mRollBrakeTimeTop = al::findActorParamS32(actor, "[転がし投げ]ブレーキ時間[上]");
        mRollBrakeTimeBottom = al::findActorParamS32(actor, "[転がし投げ]ブレーキ時間[下]");
        mRollGroundGroundedPoseTrack = al::findActorParamF32(actor, "[転がし投げ]姿勢追従[接地]");
        mRollGroundAerialPoseTrack = al::findActorParamF32(actor, "[転がし投げ]姿勢追従[空中]");
    }

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
