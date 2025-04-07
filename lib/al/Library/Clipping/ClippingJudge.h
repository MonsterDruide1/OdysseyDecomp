#pragma once

#include <basis/seadTypes.h>
#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class SceneCameraInfo;
class ClippingFarAreaObserver;

class ClippingJudge {
public:
    ClippingJudge(const ClippingFarAreaObserver*, const SceneCameraInfo*);
    bool isInClipFrustum(const sead::Vector3f&, f32, f32, s32) const;
    bool isInClipFrustumAllView(const sead::Vector3f&, f32, f32) const;
    bool isInClipFrustumObb(const sead::Matrix34f*, const sead::BoundBox3f&, f32, s32) const;
    bool isJudgedToClipFrustum(const sead::Vector3f&, f32, f32, s32) const;
    bool isJudgeToClipFrustumCore(const sead::Vector3f&, f32, f32) const;
    bool isJudgedToClipFrustumCoreObb(const sead::Matrix34f*, const sead::BoundBox3f&, f32) const;
    bool isJudgedToClipFrustumCoreObb(const sead::Matrix34f*, const sead::BoundBox3f&, f32,
                                      f32) const;
    bool isJudgedToClipFrustumObb(const sead::Matrix34f*, const sead::BoundBox3f&, f32, s32) const;
    bool isJudgedToClipFrustumUpUseFarLevel(const sead::Vector3f&, f32, f32) const;
    bool isJudgedToClipFrustumUpUseFarLevelObb(const sead::Matrix34f*, const sead::BoundBox3f&,
                                               f32) const;
    void update();

private:
    void* filler[0x3];
};

}  // namespace al
