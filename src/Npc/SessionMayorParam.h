#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/ActorParamHolder.h"

struct MusicianCameraParams {
    sead::Vector3f offset;
    al::ActorParamF32* distance;
    al::ActorParamF32* horizontalAngle;
    al::ActorParamF32* verticalAngle;
};

class SessionMayorParam {
public:
    SessionMayorParam(al::LiveActor* actor);

    const MusicianCameraParams* getDrummerCameraParams() const { return &mDrummerCameraParams; }

    const MusicianCameraParams* getBassistCameraParams() const { return &mBassistCameraParams; }

    const MusicianCameraParams* getGuitaristCameraParams() const { return &mGuitaristCameraParams; }

    const MusicianCameraParams* getTrumpeterCameraParams() const { return &mTrumpeterCameraParams; }

private:
    MusicianCameraParams mDrummerCameraParams;
    MusicianCameraParams mBassistCameraParams;
    MusicianCameraParams mGuitaristCameraParams;
    MusicianCameraParams mTrumpeterCameraParams;
};
