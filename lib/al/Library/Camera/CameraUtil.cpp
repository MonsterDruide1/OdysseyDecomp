#include "Library/Camera/CameraUtil.h"

#include "Library/Camera/CameraDirector.h"
#include "Library/Camera/CameraPoseUpdater.h"
#include "Library/Camera/CameraPoser.h"
#include "Library/Camera/CameraPoserFix.h"
#include "Library/Camera/CameraSwitchRequester.h"
#include "Library/Camera/CameraTicket.h"
#include "Library/Camera/CameraViewCtrl.h"
#include "Library/Camera/CameraViewInfo.h"
#include "Library/Camera/IUseCamera.h"
#include "Library/Camera/SceneCameraInfo.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Projection/Projection.h"

namespace al {

inline CameraDirector* getCameraDirector(const IUseCamera* user) {
    return user->getCameraDirector();
}

inline void vectorNeg(sead::Vector3f* vector) {
    vector->x = -vector->x;
    vector->y = -vector->y;
    vector->z = -vector->z;
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
    return *info->getViewAt(viewIdx)->getProjMtx();
}

const sead::Matrix44f* getProjectionMtxPtr(const IUseCamera* user, s32 viewIdx) {
    return getProjectionMtxPtr(getSceneCameraInfo(user), viewIdx);
}

const sead::Matrix44f* getProjectionMtxPtr(const SceneCameraInfo* info, s32 viewIdx) {
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

    if (isNearZero(fovy, 0.001f) || isNearZero(fovy2, 0.001f))
        return 0;

    return fovy / fovy2;
}

void calcCameraFront(sead::Vector3f* vec, const IUseCamera* user, s32 viewIdx) {
    vec->set(getCameraAt(user, viewIdx) - getCameraPos(user, viewIdx));
    normalize(vec);
}

void setNearClipDistance(const IUseCamera* user, f32 distance, s32 updaterIdx) {
    getCameraDirector(user)->getPoseUpdater(updaterIdx)->setNearClipDistance(distance);
}

void setFarClipDistance(const IUseCamera* user, f32 distance, s32 updaterIdx) {
    getCameraDirector(user)->getPoseUpdater(updaterIdx)->setFarClipDistance(distance);
}

void calcCameraDir(sead::Vector3f* dir, const IUseCamera* user, s32 viewIdx) {
    const sead::Matrix34f& mtx = getLookAtCamera(user, viewIdx).getMatrix();
    dir->set(mtx.m[2][0], mtx.m[2][1], mtx.m[2][2]);
}

void calcCameraLookDir(sead::Vector3f* dir, const IUseCamera* user, s32 viewIdx) {
    const sead::Matrix34f& mtx = getLookAtCamera(user, viewIdx).getMatrix();
    dir->set(mtx.m[2][0], mtx.m[2][1], mtx.m[2][2]);
    vectorNeg(dir);
}

void calcCameraSideDir(sead::Vector3f* sideDir, const IUseCamera* user, s32 viewIdx) {
    return getLookAtCamera(user, viewIdx).getRightVectorByMatrix(sideDir);
}

void calcCameraUpDir(sead::Vector3f* upDir, const IUseCamera* user, s32 viewIdx) {
    return getLookAtCamera(user, viewIdx).getUpVectorByMatrix(upDir);
}

bool tryCalcCameraDir(sead::Vector3f* dir, const SceneCameraInfo* info, s32 viewIdx) {
    const sead::Matrix34f& mtx = info->getViewAt(viewIdx)->getLookAtCam().getMatrix();
    dir->set(mtx.m[2][0], mtx.m[2][1], mtx.m[2][2]);

    return tryNormalizeOrZero(dir);
}

bool tryCalcCameraDirH(sead::Vector3f* dirH, const SceneCameraInfo* info,
                       const sead::Vector3f& upDir, s32 viewIdx) {
    const sead::Matrix34f& mtx = info->getViewAt(viewIdx)->getLookAtCam().getMatrix();
    verticalizeVec(dirH, upDir, {mtx.m[2][0], mtx.m[2][1], mtx.m[2][2]});

    return tryNormalizeOrZero(dirH);
}

bool tryCalcCameraLookDirH(sead::Vector3f* dir, const SceneCameraInfo* info,
                           const sead::Vector3f& upDir, s32 viewIdx) {
    if (!tryCalcCameraDirH(dir, info, upDir, viewIdx))
        return false;

    dir->set(dir->x, dir->y, dir->z);
    vectorNeg(dir);
    return true;
}

void startCamera(const IUseCamera* user, CameraTicket* ticket, s32 num) {
    getCameraDirector(user)->getSceneCameraCtrl()->getViewAt(0)->getSwitchRequester()->requestStart(
        ticket, num);
}

void startCameraSub(const IUseCamera* user, CameraTicket* ticket, s32 num) {
    getCameraDirector(user)->getSceneCameraCtrl()->getViewAt(1)->getSwitchRequester()->requestStart(
        ticket, num);
}

bool isActiveCamera(const CameraTicket* ticket) {
    return ticket->getActive();
}

CameraTicket* initObjectCamera(const IUseCamera* user, const PlacementInfo& placementInfo,
                               const char* str0, const char* str1) {
    sead::Matrix34f mat;
    memcpy(&mat, &sead::Matrix34f::ident, sizeof(mat));
    tryGetZoneMatrixTR(&mat, placementInfo);

    return getCameraDirector(user)->createObjectCamera(createPlacementId(placementInfo), str0, str1,
                                                       5, mat);
}

CameraTicket* initObjectCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                               const char* str0, const char* str1) {
    const PlacementInfo* placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f mat;
    memcpy(&mat, &sead::Matrix34f::ident, sizeof(mat));
    tryGetZoneMatrixTR(&mat, *placementInfo);

    return getCameraDirector(user)->createObjectCamera(createPlacementId(*placementInfo), str0,
                                                       str1, 5, mat);
}

CameraTicket* initObjectCameraNoPlacementInfo(const IUseCamera* user, const char* str0,
                                              const char* str1) {
    return getCameraDirector(user)->createObjectCamera(nullptr, str0, str1, 5,
                                                       sead::Matrix34f::ident);
}

CameraTicket* initFixCamera(const IUseCamera* user, const char* str, const sead::Vector3f& camPos,
                            const sead::Vector3f& lookAtPos) {
    CameraPoserFix* poserFix = new CameraPoserFix("固定");

    CameraTicket* ticket = getCameraDirector(user)->createCamera(
        poserFix->getCameraPoser(), nullptr, str, 5, sead::Matrix34f::ident, true);

    poserFix->initCameraPosAndLookAtPos(camPos, lookAtPos);
    return ticket;
}

CameraTicket* initFixDoorwayCamera(const IUseCamera* user, const char* str,
                                   const sead::Vector3f& camPos, const sead::Vector3f& lookAtPos) {
    CameraPoserFix* poser = new CameraPoserFix(CameraPoserFix::getFixDoorwayCameraName());

    CameraTicket* ticket = getCameraDirector(user)->createCamera(
        poser->getCameraPoser(), nullptr, str, 5, sead::Matrix34f::ident, true);

    poser->initCameraPosAndLookAtPos(camPos, lookAtPos);
    return ticket;
}

}  // namespace al
