#pragma once

#include <basis/seadTypes.h>
#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class FrustumRadar;
class SceneCameraInfo;
class ClippingFarAreaObserver;

// TODO: rename `idx`, `idy` and `idz` across all functions
class ClippingJudge {
public:
    ClippingJudge(const ClippingFarAreaObserver* clippingFarAreaObserver,
                  const SceneCameraInfo* cameraInfo);

    void update();
    bool isJudgedToClipFrustumUnUseFarLevel(const sead::Vector3f& pos, f32 idx, f32 idy) const;
    bool isJudgedToClipFrustumCore(const sead::Vector3f& pos, f32 idx, f32 idy) const;
    bool isJudgedToClipFrustum(const sead::Vector3f& pos, f32 idx, f32 idy, s32 idz) const;
    bool isJudgedToClipFrustumCore(const sead::Vector3f& pos, f32 idx, f32 idy, f32 idz) const;
    bool isInClipFrustum(const sead::Vector3f& pos, f32 idx, f32 idy, s32 idz) const;
    bool isInClipFrustumAllView(const sead::Vector3f& pos, f32 idx, f32 idy) const;
    bool isJudgedToClipFrustumUnUseFarLevelObb(const sead::Matrix34f* mtx,
                                               const sead::BoundBox3f& bound, f32 idx) const;
    bool isJudgedToClipFrustumCoreObb(const sead::Matrix34f* mtx, const sead::BoundBox3f& bound,
                                      f32 idx) const;
    bool isInClipFrustumObb(const sead::Matrix34f* mtx, const sead::BoundBox3f& bound, f32 idx,
                            s32 idy) const;
    bool isJudgedToClipFrustumCoreObb(const sead::Matrix34f* mtx, const sead::BoundBox3f& bound,
                                      f32 idx, f32 idy) const;
    bool isJudgedToClipFrustumObb(const sead::Matrix34f* mtx, const sead::BoundBox3f& bound,
                                  f32 idx, s32 idy) const;

private:
    const ClippingFarAreaObserver* mFarAreaObserver;
    FrustumRadar** mFrustumRadars = nullptr;
    const SceneCameraInfo* mCameraInfo;
};

}  // namespace al
