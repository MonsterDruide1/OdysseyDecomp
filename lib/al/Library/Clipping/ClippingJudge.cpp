#include "Library/Clipping/ClippingJudge.h"

#include "Library/Camera/CameraViewInfo.h"
#include "Library/Camera/SceneCameraInfo.h"
#include "Library/Clipping/FrustumRadar.h"
#include "Project/Clipping/ClippingFarAreaObserver.h"

namespace al {

ClippingJudge::ClippingJudge(const ClippingFarAreaObserver* clippingFarAreaObserver,
                             const SceneCameraInfo* cameraInfo)
    : mFarAreaObserver(clippingFarAreaObserver), mCameraInfo(cameraInfo) {
    mFrustumRadars = new FrustumRadar*[mCameraInfo->getViewNumMax()];
    for (s32 i = 0; i < mCameraInfo->getViewNumMax(); i++)
        mFrustumRadars[i] = new FrustumRadar();
}

void ClippingJudge::update() {
    for (s32 i = 0; i < mCameraInfo->getViewNumMax(); i++) {
        CameraViewInfo* cameraViewInfo = mCameraInfo->getViewAt(i);
        if (!cameraViewInfo->isValid())
            continue;
        FrustumRadar* frustumRadar = mFrustumRadars[i];
        const sead::Matrix34f& camMtx = cameraViewInfo->getLookAtCam().getMatrix();
        const sead::Matrix44f& projMtx = cameraViewInfo->getProjMtxStd();
        frustumRadar->calcFrustumArea(camMtx, projMtx, 300.0f,
                                      mFarAreaObserver->getFarClipDistance());
    }
}

bool ClippingJudge::isJudgedToClipFrustumUnUseFarLevel(const sead::Vector3f& pos, f32 idx,
                                                       f32 idy) const {
    return isJudgedToClipFrustumCore(pos, idx, idy);
}

bool ClippingJudge::isJudgedToClipFrustumCore(const sead::Vector3f& pos, f32 idx, f32 idy) const {
    for (s32 i = 0; i < mCameraInfo->getViewNumMax(); i++) {
        if (mCameraInfo->getViewAt(i)->isValid()) {
            if (mFrustumRadars[i]->judgeInArea(pos, idx, idy))
                return false;
        }
    }
    return true;
}

bool ClippingJudge::isJudgedToClipFrustum(const sead::Vector3f& pos, f32 idx, f32 idy,
                                          s32 idz) const {
    if (idz == 0)
        return isJudgedToClipFrustumCore(pos, idx, idy, -1.0f);
    return isJudgedToClipFrustumCore(pos, idx, idy);
}

bool ClippingJudge::isJudgedToClipFrustumCore(const sead::Vector3f& pos, f32 idx, f32 idy,
                                              f32 idz) const {
    for (s32 i = 0; i < mCameraInfo->getViewNumMax(); i++) {
        if (mCameraInfo->getViewAt(i)->isValid()) {
            if (mFrustumRadars[i]->judgeInArea(pos, idx, idy, idz))
                return false;
        }
    }
    return true;
}

bool ClippingJudge::isInClipFrustum(const sead::Vector3f& pos, f32 idx, f32 idy, s32 idz) const {
    return mFrustumRadars[idz]->judgeInArea(pos, idx, idy);
}

bool ClippingJudge::isInClipFrustumAllView(const sead::Vector3f& pos, f32 idx, f32 idy) const {
    return !isJudgedToClipFrustumCore(pos, idx, idy);
}

bool ClippingJudge::isJudgedToClipFrustumUnUseFarLevelObb(const sead::Matrix34f* mtx,
                                                          const sead::BoundBox3f& bound,
                                                          f32 idx) const {
    return isJudgedToClipFrustumCoreObb(mtx, bound, idx);
}

bool ClippingJudge::isJudgedToClipFrustumCoreObb(const sead::Matrix34f* mtx,
                                                 const sead::BoundBox3f& bound, f32 idx) const {
    for (s32 i = 0; i < mCameraInfo->getViewNumMax(); i++) {
        if (mCameraInfo->getViewAt(i)->isValid()) {
            if (mFrustumRadars[i]->judgeInAreaObb(mtx, bound, idx))
                return false;
        }
    }
    return true;
}

bool ClippingJudge::isInClipFrustumObb(const sead::Matrix34f* mtx, const sead::BoundBox3f& bound,
                                       f32 idx, s32 idy) const {
    return mFrustumRadars[idy]->judgeInAreaObb(mtx, bound, idx);
}

bool ClippingJudge::isJudgedToClipFrustumCoreObb(const sead::Matrix34f* mtx,
                                                 const sead::BoundBox3f& bound, f32 idx,
                                                 f32 idy) const {
    for (s32 i = 0; i < mCameraInfo->getViewNumMax(); i++) {
        if (mCameraInfo->getViewAt(i)->isValid()) {
            if (mFrustumRadars[i]->judgeInAreaObb(mtx, bound, idx, idy))
                return false;
        }
    }
    return true;
}

bool ClippingJudge::isJudgedToClipFrustumObb(const sead::Matrix34f* mtx,
                                             const sead::BoundBox3f& bound, f32 idx,
                                             s32 idy) const {
    if (idy == 0)
        return isJudgedToClipFrustumCoreObb(mtx, bound, idx, -1.0f);
    return isJudgedToClipFrustumCoreObb(mtx, bound, idx);
}

}  // namespace al
