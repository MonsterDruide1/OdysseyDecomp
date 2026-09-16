#pragma once

#include <gfx/seadCamera.h>
#include <math/seadMatrix.h>

namespace al {

class CameraViewFlag;
struct OrthoProjectionInfo;
class Projection;

class CameraViewInfo {
public:
    CameraViewInfo(s32 index, const sead::LookAtCamera& lookAtCam, const Projection& projection,
                   const CameraViewFlag& flag, const OrthoProjectionInfo& orthoProjectionInfo);

    const sead::Projection& getProjectionSead() const;
    const sead::Matrix44f& getProjMtx() const;
    const sead::Matrix44f& getProjMtxStd() const;
    f32 getAspect() const;
    f32 getNear() const;
    f32 getFar() const;

    s32 getIndex() const { return mIndex; }

    bool isValid() const { return mIsValid; }

    void setValid(bool valid) { mIsValid = valid; }

    bool isFirstCalc() const { return mIsFirstCalc; }

    void setFirstCalc(bool firstCalc) { mIsFirstCalc = firstCalc; }

    bool isActiveInterpole() const { return mIsActiveInterpole; }

    void setActiveInterpole(bool active) { mIsActiveInterpole = active; }

    const sead::LookAtCamera& getLookAtCam() const { return mLookAtCam; }

    const Projection& getProjection() const { return mProjection; }

private:
    s32 mIndex;
    bool mIsValid = true;
    bool mIsFirstCalc = true;
    bool mIsActiveInterpole = false;
    const sead::LookAtCamera& mLookAtCam;
    const Projection& mProjection;
    const CameraViewFlag& mViewFlag;
    const OrthoProjectionInfo& mOrthoProjectionInfo;
};
}  // namespace al
