#include "game/hack/HackCapThrowParam.h"

#include "al/util/LiveActorUtil.h"

HackCapThrowParam::HackCapThrowParam(al::LiveActor* actor) {
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
