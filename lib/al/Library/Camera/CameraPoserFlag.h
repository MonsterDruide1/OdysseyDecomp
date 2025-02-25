#pragma once

#include <basis/seadTypes.h>

namespace al {

class ByamlIter;

struct CameraPoserFlag {
    CameraPoserFlag();

    void load(const ByamlIter& iter);
    bool isValidKeepPreSelfPoseNextCamera() const;

    bool mIsFirstCalc = true;
    bool mIsOffVerticalAbsorb = false;
    bool mIsInvalidCollider = true;
    bool _3 = false;
    bool mIsValidKeepPreSelfPoseNextCameraByParam = false;
    bool mIsOverWriteProgram = false;
    bool mIsInvalidKeepPreSelfPoseNextCameraOverWriteProgram = false;
    bool mIsInvalidKeepDistanceNextCamera = false;
    bool mIsInvalidKeepDistanceNextCameraIfNoCollide = false;
    bool mIsValidCtrlSubjective = false;
    bool mIsInvalidChangeSubjective = false;
    bool mIsInvalidCameraBlur = false;
    bool _c = false;
    bool mIsInvalidPreCameraEndAfterInterpole = false;
    bool mIsStopUpdateGyro = false;
};

static_assert(sizeof(CameraPoserFlag) == 0xF);

}  // namespace al
