#pragma once

namespace al {
class ByamlIter;

struct CameraPoserFlag {
    bool mIsFirstCalc;
    bool mIsOffVerticalAbsorb;
    bool mIsInvalidCollider;
    bool _3;
    bool mIsValidKeepPreSelfPoseNextCameraByParam;
    bool _5;
    bool mIsInvalidKeepPreSelfPoseNextCameraOverWriteProgram;
    bool mIsInvalidKeepDistanceNextCamera;
    bool mIsInvalidKeepDistanceNextCameraIfNoCollide;
    bool mIsValidCtrlSubjective;
    bool mIsInvalidChangeSubjective;
    bool mIsInvalidCameraBlur;
    bool _C;
    bool mIsInvalidPreCameraEndAfterInterpole;
    bool mIsStopUpdateGyro;

public:
    CameraPoserFlag();
    void load(const al::ByamlIter& iter);
    bool isValidKeepPreSelfPoseNextCamera() const;
};

}  // namespace al
