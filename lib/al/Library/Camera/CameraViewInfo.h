#pragma once

#include <gfx/seadCamera.h>
#include <math/seadMatrix.h>

namespace al {

class Projection;
class CameraViewFlag;
struct OrthoProjectionInfo;

class CameraViewInfo {
public:
    CameraViewInfo(s32 num, const sead::LookAtCamera& lookAtCam, const Projection& projection,
                   const CameraViewFlag& flag, const OrthoProjectionInfo& orthoProjectionInfo)
        : mIndex(num), _6(false), _5(true), mIsValid(true), mLookAtCam(lookAtCam),
          mProjection(projection), mCamViewFlag(flag), mOrthoProjectionInfo(orthoProjectionInfo) {}

    const Projection& getProjectionSead() const;
    const sead::Matrix44f& getProjMtx() const;
    const sead::Matrix44f& getProjMtxStd() const;
    f32 getAspect() const;
    f32 getNear() const;
    f32 getFar() const;

    s32 getIndex() const { return mIndex; }

    bool getIsValid() const { return mIsValid; }

    const sead::LookAtCamera& getLookAtCam() const { return mLookAtCam; }

private:
    s32 mIndex;
    bool mIsValid;
    bool _5;
    bool _6;
    const sead::LookAtCamera& mLookAtCam;
    const Projection& mProjection;
    const CameraViewFlag& mCamViewFlag;
    const OrthoProjectionInfo& mOrthoProjectionInfo;
};
}  // namespace al
