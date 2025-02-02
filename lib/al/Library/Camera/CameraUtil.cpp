#include "Library/Camera/CameraUtil.h"

#include "Library/Camera/CameraDirector.h"
#include "Library/Camera/CameraViewInfo.h"
#include "Library/Camera/IUseCamera.h"
#include "Library/Camera/SceneCameraInfo.h"

namespace al {

SceneCameraInfo* getSceneCameraInfo(const IUseCamera* user) {
    return user->getCameraDirector()->getSceneCameraInfo();
}

s32 getViewNumMax(const IUseCamera* user) {
    return getViewNumMax(getSceneCameraInfo(user));
}

s32 getViewNumMax(const SceneCameraInfo* info) {
    return info->getViewNumMax();
}

bool isValidView(const IUseCamera* user, s32 viewIdx) {
    return isValidView(getSceneCameraInfo(user), viewIdx);
}

bool isValidView(const SceneCameraInfo* info, s32 viewIdx) {
    return info->getViewAt(viewIdx)->getIsValid();
}

const char* getViewName(const IUseCamera* user, s32 viewIdx) {
    return getViewName(getSceneCameraInfo(user), viewIdx);
}

const char* getViewName(const SceneCameraInfo* info, s32 viewIdx) {
    return info->getViewName(viewIdx);
}

const sead::Matrix34f& getViewMtx(const IUseCamera* user, s32 viewIdx) {
    return getViewMtx(getSceneCameraInfo(user), viewIdx);
}

const sead::Matrix34f& getViewMtx(const SceneCameraInfo* info, s32 viewIdx) {
    return info->getViewAt(viewIdx)->getLookAtCam()->getMatrix();
}

/*sead::Matrix34f* getViewMtxPtr(const IUseCamera* user, s32 viewIdx) {
    getViewMtxPtr(getSceneCameraInfo(user), viewIdx);
}*/ // if the lower doesnt match this doesnt either

// sead::Matrix34f* getViewMtxPtr(const SceneCameraInfo* info, s32 viewIdx) {}

}  // namespace al
