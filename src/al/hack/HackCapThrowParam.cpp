#include "al/hack/HackCapThrowParam.h"

#include "al/util/LiveActorUtil.h"

HackCapThrowParam::HackCapThrowParam(al::LiveActor* actor) {
    hackThrowHeight = al::findActorParamF32(actor, "投げる高さ");
    maxVel = al::findActorParamF32(actor, "最高速度");
    continuousThrowSpeed = al::findActorParamF32(actor, "連続投げ速度");
    breakTime = al::findActorParamS32(actor, "ブレーキ時間");
    maxDist = al::findActorParamF32(actor, "到達距離");
    endpointStopTime = al::findActorParamS32(actor, "端点停止時間");
    maxEndpointStopTime = al::findActorParamS32(actor, "最大端点停止時間");
    returnStrength = al::findActorParamF32(actor, "戻り強さ");
    maxRetSpeed = al::findActorParamF32(actor, "戻り最高速度");
    turnAngleLimit = al::findActorParamF32(actor, "ターン限界角度");
    waterMaxSpeed = al::findActorParamF32(actor, "[水中]最高速度");
    waterDist = al::findActorParamF32(actor, "[水中]到達距離");
    waterBreakTime = al::findActorParamS32(actor, "[水中]ブレーキ時間");
    waterMaxRetSpeed = al::findActorParamF32(actor, "[水中]戻り最高速度");
    tornadoDist = al::findActorParamF32(actor, "[竜巻投げ]到達距離");
    tornadoMaxDist = al::findActorParamF32(actor, "[竜巻投げ]最高到達距離");
    tornadoReflectTime = al::findActorParamS32(actor, "[竜巻投げ]反射時間");
    rollSpeed = al::findActorParamF32(actor, "[転がし投げ]速度");
    rollDistTop = al::findActorParamF32(actor, "[転がし投げ]到達距離[上]");
    rollDistBottom = al::findActorParamF32(actor, "[転がし投げ]到達距離[下]");
    rollBrakeTimeTop = al::findActorParamS32(actor, "[転がし投げ]ブレーキ時間[上]");
    rollBrakeTimeBottom = al::findActorParamS32(actor, "[転がし投げ]ブレーキ時間[下]");
    rollGroundGroundedPoseTrack = al::findActorParamF32(actor, "[転がし投げ]姿勢追従[接地]");
    rollGroundAerialPoseTrack = al::findActorParamF32(actor, "[転がし投げ]姿勢追従[空中]");
}
