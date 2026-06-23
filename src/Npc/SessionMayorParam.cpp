#include "Npc/SessionMayorParam.h"

#include "Library/LiveActor/ActorParamHolderUtil.h"

SessionMayorParam::SessionMayorParam(al::LiveActor* actor) {
    mDrummerCameraParams.offset.set(
        al::findActorParamF32(actor, "ドラム/カメラ/オフセットX")->value,
        al::findActorParamF32(actor, "ドラム/カメラ/オフセットY")->value,
        al::findActorParamF32(actor, "ドラム/カメラ/オフセットZ")->value);
    mDrummerCameraParams.distance = al::findActorParamF32(actor, "ドラム/カメラ/距離");
    mDrummerCameraParams.horizontalAngle = al::findActorParamF32(actor, "ドラム/カメラ/水平角度");
    mDrummerCameraParams.verticalAngle = al::findActorParamF32(actor, "ドラム/カメラ/垂直角度");

    mBassistCameraParams.offset.set(
        al::findActorParamF32(actor, "ベース/カメラ/オフセットX")->value,
        al::findActorParamF32(actor, "ベース/カメラ/オフセットY")->value,
        al::findActorParamF32(actor, "ベース/カメラ/オフセットZ")->value);
    mBassistCameraParams.distance = al::findActorParamF32(actor, "ベース/カメラ/距離");
    mBassistCameraParams.horizontalAngle = al::findActorParamF32(actor, "ベース/カメラ/水平角度");
    mBassistCameraParams.verticalAngle = al::findActorParamF32(actor, "ベース/カメラ/垂直角度");

    mGuitaristCameraParams.offset.set(
        al::findActorParamF32(actor, "ギター/カメラ/オフセットX")->value,
        al::findActorParamF32(actor, "ギター/カメラ/オフセットY")->value,
        al::findActorParamF32(actor, "ギター/カメラ/オフセットZ")->value);
    mGuitaristCameraParams.distance = al::findActorParamF32(actor, "ギター/カメラ/距離");
    mGuitaristCameraParams.horizontalAngle = al::findActorParamF32(actor, "ギター/カメラ/水平角度");
    mGuitaristCameraParams.verticalAngle = al::findActorParamF32(actor, "ギター/カメラ/垂直角度");

    mTrumpeterCameraParams.offset.set(
        al::findActorParamF32(actor, "トランペット/カメラ/オフセットX")->value,
        al::findActorParamF32(actor, "トランペット/カメラ/オフセットY")->value,
        al::findActorParamF32(actor, "トランペット/カメラ/オフセットZ")->value);
    mTrumpeterCameraParams.distance = al::findActorParamF32(actor, "トランペット/カメラ/距離");
    mTrumpeterCameraParams.horizontalAngle =
        al::findActorParamF32(actor, "トランペット/カメラ/水平角度");
    mTrumpeterCameraParams.verticalAngle =
        al::findActorParamF32(actor, "トランペット/カメラ/垂直角度");
}
