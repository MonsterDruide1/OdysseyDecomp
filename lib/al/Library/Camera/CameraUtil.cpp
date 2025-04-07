#include "Library/Camera/CameraUtil.h"

#include "Library/Camera/CameraDirector.h"
#include "Library/Camera/CameraViewInfo.h"
#include "Library/Camera/IUseCamera.h"
#include "Library/Camera/SceneCameraInfo.h"
#include "Library/Math/MathUtil.h"
#include "Library/Projection/Projection.h"

namespace al {

inline CameraDirector* getCameraDirector(const IUseCamera* user) {
    return user->getCameraDirector();
}

SceneCameraInfo* getSceneCameraInfo(const IUseCamera* user) {
    return getCameraDirector(user)->getSceneCameraInfo();
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
    return info->getViewAt(viewIdx)->isValid();
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
    return getLookAtCamera(info, viewIdx).getMatrix();
}

const sead::Matrix34f* getViewMtxPtr(const IUseCamera* user, s32 viewIdx) {
    return getViewMtxPtr(getSceneCameraInfo(user), viewIdx);
}

const sead::Matrix34f* getViewMtxPtr(const SceneCameraInfo* info, s32 viewIdx) {
    return &getViewMtx(info, viewIdx);
}

const sead::Matrix44f& getProjectionMtx(const IUseCamera* user, s32 viewIdx) {
    return getProjectionMtx(getSceneCameraInfo(user), viewIdx);
}

const sead::Matrix44f& getProjectionMtx(const SceneCameraInfo* info, s32 viewIdx) {
    return info->getViewAt(viewIdx)->getProjMtx();
}

const sead::LookAtCamera& getLookAtCamera(const IUseCamera* user, s32 viewIdx) {
    return getLookAtCamera(getSceneCameraInfo(user), viewIdx);
}

const sead::LookAtCamera& getLookAtCamera(const SceneCameraInfo* info, s32 viewIdx) {
    return info->getViewAt(viewIdx)->getLookAtCam();
}

const sead::Projection& getProjectionSead(const IUseCamera* user, s32 viewIdx) {
    return getProjectionSead(getSceneCameraInfo(user), viewIdx);
}

const sead::Projection& getProjectionSead(const SceneCameraInfo* info, s32 viewIdx) {
    return info->getViewAt(viewIdx)->getProjectionSead();
}

const Projection& getProjection(const IUseCamera* user, s32 viewIdx) {
    return getProjection(getSceneCameraInfo(user), viewIdx);
}

const Projection& getProjection(const SceneCameraInfo* info, s32 viewIdx) {
    return info->getViewAt(viewIdx)->getProjection();
}

const sead::Vector3f& getCameraPos(const IUseCamera* user, s32 viewIdx) {
    return getCameraPos(getSceneCameraInfo(user), viewIdx);
}

const sead::Vector3f& getCameraPos(const SceneCameraInfo* info, s32 viewIdx) {
    return getLookAtCamera(info, viewIdx).getPos();
}

const sead::Vector3f& getCameraAt(const IUseCamera* user, s32 viewIdx) {
    return getCameraAt(getSceneCameraInfo(user), viewIdx);
}

const sead::Vector3f& getCameraAt(const SceneCameraInfo* info, s32 viewIdx) {
    return getLookAtCamera(info, viewIdx).getAt();
}

const sead::Vector3f& getCameraUp(const IUseCamera* user, s32 viewIdx) {
    return getCameraUp(getSceneCameraInfo(user), viewIdx);
}

const sead::Vector3f& getCameraUp(const SceneCameraInfo* info, s32 viewIdx) {
    return getLookAtCamera(info, viewIdx).getUp();
}

f32 getFovyDegree(const IUseCamera* user, s32 viewIdx) {
    return getFovyDegree(getSceneCameraInfo(user), viewIdx);
}

f32 getFovyDegree(const SceneCameraInfo* info, s32 viewIdx) {
    return sead::Mathf::rad2deg(getFovy(info, viewIdx));
}

f32 getFovy(const IUseCamera* user, s32 viewIdx) {
    return getFovy(getSceneCameraInfo(user), viewIdx);
}

f32 getFovy(const SceneCameraInfo* info, s32 viewIdx) {
    return info->getViewAt(viewIdx)->getProjection().getFovy();
}

f32 getNear(const IUseCamera* user, s32 viewIdx) {
    return getNear(getSceneCameraInfo(user), viewIdx);
}

f32 getNear(const SceneCameraInfo* info, s32 viewIdx) {
    return info->getViewAt(viewIdx)->getNear();
}

f32 getFar(const IUseCamera* user, s32 viewIdx) {
    return getFar(getSceneCameraInfo(user), viewIdx);
}

f32 getFar(const SceneCameraInfo* info, s32 viewIdx) {
    return info->getViewAt(viewIdx)->getFar();
}

f32 calcCameraDistance(const IUseCamera* user, s32 viewIdx) {
    sead::Vector3f diff = getCameraPos(user, viewIdx) - getCameraAt(user, viewIdx);

    return diff.length();
}

f32 calcFovxDegree(const IUseCamera* user, s32 viewIdx) {
    f32 aspect = getSceneCameraInfo(user)->getViewAt(viewIdx)->getAspect();

    return aspect * getFovyDegree(user, viewIdx);
}

f32 calcCurrentFovyRate(const IUseCamera* user, s32 viewIdx) {
    f32 fovy = getFovyDegree(user, viewIdx);
    f32 fovy2 = getCameraDirector(user)->getSceneFovyDegree();

    if (isNearZero(fovy) || isNearZero(fovy2))
        return 0;

    return fovy / fovy2;
}
}  // namespace al
