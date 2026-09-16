#pragma once

#include <gfx/seadCamera.h>
#include <math/seadMatrix.h>

namespace al {

class CameraViewFlag;
struct OrthoProjectionInfo;
class Projection;

struct CameraViewInfo {
    CameraViewInfo(s32 index, const sead::LookAtCamera& lookAtCam, const Projection& projection,
                   const CameraViewFlag& flag, const OrthoProjectionInfo& orthoProjectionInfo);

    const sead::Projection& getProjectionSead() const;
    const sead::Matrix44f& getProjMtx() const;
    const sead::Matrix44f& getProjMtxStd() const;
    f32 getAspect() const;
    f32 getNear() const;
    f32 getFar() const;

    s32 index;
    bool isValid = true;
    bool isFirstCalc = true;
    bool isActiveInterpole = false;

    const sead::LookAtCamera& lookAtCam;
    const Projection& projection;
    const CameraViewFlag& flag;
    const OrthoProjectionInfo& orthoProjectionInfo;
};
}  // namespace al
