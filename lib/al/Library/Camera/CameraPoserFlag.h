#pragma once

#include <basis/seadTypes.h>

namespace al {

class ByamlIter;

class CameraPoserFlag {
public:
    CameraPoserFlag();

    void load(const ByamlIter& iter);
    bool isValidKeepPreSelfPoseNextCamera();

    bool mIsFirstCalc = true;
    bool mIsOffVerticalAbsorb = false;
    bool mIsInvalidCollider = true;
    bool _3 = false;
    bool mIsValidKeepPreSelfPoseNextCameraByParam = false;
    bool _5 = false;
    bool mIsInvalidKeepPreSelfPoseNextCameraOverWriteProgram = false;
    bool mIsInvalidKeepDistanceNextCamera = false;
    bool _8 = false;
    bool _9 = false;
    bool mIsInvalidChangeSubjective = false;
    bool _11 = false;
    bool _12 = false;
    bool mIsInvalidPreCameraEndAfterInterpole = false;
    bool mIsStopUpdateGyro = false;
};  // they are get and set all over anyways, so public?

static_assert(sizeof(CameraPoserFlag) == 0xF);
}  // namespace al
