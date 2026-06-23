#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/ActorParamHolder.h"
#include "Library/LiveActor/ActorParamHolderUtil.h"

struct MusicianCameraParams {
    sead::Vector3f offset;
    al::ActorParamF32* distance;
    al::ActorParamF32* horizontalAngle;
    al::ActorParamF32* verticalAngle;
};

struct SessionMayorParam {
    SessionMayorParam(al::LiveActor* actor) {
        drummerCameraParams.offset.set(
            al::findActorParamF32(actor, "ドラム/カメラ/オフセットX")->value,
            al::findActorParamF32(actor, "ドラム/カメラ/オフセットY")->value,
            al::findActorParamF32(actor, "ドラム/カメラ/オフセットZ")->value);
        drummerCameraParams.distance = al::findActorParamF32(actor, "ドラム/カメラ/距離");
        drummerCameraParams.horizontalAngle =
            al::findActorParamF32(actor, "ドラム/カメラ/水平角度");
        drummerCameraParams.verticalAngle = al::findActorParamF32(actor, "ドラム/カメラ/垂直角度");

        bassistCameraParams.offset.set(
            al::findActorParamF32(actor, "ベース/カメラ/オフセットX")->value,
            al::findActorParamF32(actor, "ベース/カメラ/オフセットY")->value,
            al::findActorParamF32(actor, "ベース/カメラ/オフセットZ")->value);
        bassistCameraParams.distance = al::findActorParamF32(actor, "ベース/カメラ/距離");
        bassistCameraParams.horizontalAngle =
            al::findActorParamF32(actor, "ベース/カメラ/水平角度");
        bassistCameraParams.verticalAngle = al::findActorParamF32(actor, "ベース/カメラ/垂直角度");

        guitaristCameraParams.offset.set(
            al::findActorParamF32(actor, "ギター/カメラ/オフセットX")->value,
            al::findActorParamF32(actor, "ギター/カメラ/オフセットY")->value,
            al::findActorParamF32(actor, "ギター/カメラ/オフセットZ")->value);
        guitaristCameraParams.distance = al::findActorParamF32(actor, "ギター/カメラ/距離");
        guitaristCameraParams.horizontalAngle =
            al::findActorParamF32(actor, "ギター/カメラ/水平角度");
        guitaristCameraParams.verticalAngle =
            al::findActorParamF32(actor, "ギター/カメラ/垂直角度");

        trumpeterCameraParams.offset.set(
            al::findActorParamF32(actor, "トランペット/カメラ/オフセットX")->value,
            al::findActorParamF32(actor, "トランペット/カメラ/オフセットY")->value,
            al::findActorParamF32(actor, "トランペット/カメラ/オフセットZ")->value);
        trumpeterCameraParams.distance = al::findActorParamF32(actor, "トランペット/カメラ/距離");
        trumpeterCameraParams.horizontalAngle =
            al::findActorParamF32(actor, "トランペット/カメラ/水平角度");
        trumpeterCameraParams.verticalAngle =
            al::findActorParamF32(actor, "トランペット/カメラ/垂直角度");
    }

    MusicianCameraParams drummerCameraParams;
    MusicianCameraParams bassistCameraParams;
    MusicianCameraParams guitaristCameraParams;
    MusicianCameraParams trumpeterCameraParams;
};

static_assert(sizeof(SessionMayorParam) == 0xA0);
