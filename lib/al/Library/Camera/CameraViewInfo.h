#pragma once

#include <gfx/seadCamera.h>
#include <math/seadMatrix.h>

namespace al {

class CameraViewFlag {
public:
    CameraViewFlag();

    void resetAllFlag();

private:
    bool _0 = false;
};

class Projection;
struct OrthoProjectionInfo;

class CameraViewInfo {
public:
    CameraViewInfo(s32 num, const sead::LookAtCamera& lookAtCam, const Projection& projection,
                   const CameraViewFlag& flag, const OrthoProjectionInfo& orthoProjectionInfo);

    const sead::Projection& getProjectionSead() const;
    const sead::Matrix44f& getProjMtx() const;
    const sead::Matrix44f& getProjMtxStd() const;
    f32 getAspect() const;
    f32 getNear() const;
    f32 getFar() const;

    s32 getIndex() const { return mIndex; }

    bool getIsValid() const { return mIsValid; }

    const sead::LookAtCamera& getLookAtCam() const { return mLookAtCam; }

    const Projection& getProjection() const { return mProjection; }

private:
    s32 mIndex;
    bool mIsValid = true;
    bool _5 = true;
    bool _6 = false;
    const sead::LookAtCamera& mLookAtCam;
    const Projection& mProjection;
    const CameraViewFlag& mCamViewFlag;
    const OrthoProjectionInfo& mOrthoProjectionInfo;
};
}  // namespace al
