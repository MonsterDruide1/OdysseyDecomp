#pragma once

#include <basis/seadTypes.h>

namespace al {

class ByamlIter;

struct CameraPoserFlag {
    CameraPoserFlag();

    void load(const ByamlIter& iter);
    bool isValidKeepPreSelfPoseNextCamera() const;

    bool isFirstCalc = true;
    bool isOffVerticalAbsorb = false;
    bool isInvalidCollider = true;
    bool _3 = false;
    bool isValidKeepPreSelfPoseNextCameraByParam = false;
    bool isOverWriteProgram = false;
    bool isInvalidKeepPreSelfPoseNextCameraOverWriteProgram = false;
    bool isInvalidKeepDistanceNextCamera = false;
    bool isInvalidKeepDistanceNextCameraIfNoCollide = false;
    bool isValidCtrlSubjective = false;
    bool isInvalidChangeSubjective = false;
    bool isInvalidCameraBlur = false;
    bool _c = false;
    bool isInvalidPreCameraEndAfterInterpole = false;
    bool isStopUpdateGyro = false;
};

static_assert(sizeof(CameraPoserFlag) == 0xF);

}  // namespace al
