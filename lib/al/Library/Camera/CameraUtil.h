#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
// TODO: Add functions.
class IUseCamera;
class Projection;
class CameraSubTargetBase;
class CameraSubTargetTurnParam;
class LiveActor;
class SceneCameraInfo;

bool tryCalcCameraLookDirH(sead::Vector3f*, const SceneCameraInfo*, const sead::Vector3f&, s32);

Projection getProjection(const IUseCamera* cameraHolder, s32 cameraNum);

const sead::Vector3f& getCameraPos(const IUseCamera* cameraHolder, s32 cameraNum);
void calcCameraUpDir(sead::Vector3f* out, const IUseCamera* cameraHolder, s32 cameraNum);
s32 getViewNumMax(const IUseCamera* cameraHolder);
CameraSubTargetBase* createActorCameraSubTarget(const LiveActor* actor, const sead::Vector3f*);
void initCameraSubTargetTurnParam(CameraSubTargetBase* cameraSubTarget,
                                  const CameraSubTargetTurnParam* params);
void setCameraPlacementSubTarget(IUseCamera* cameraHolder, CameraSubTargetBase* cameraSubTarget);
void resetCameraPlacementSubTarget(IUseCamera* cameraHolder, CameraSubTargetBase* cameraSubTarget);
const sead::Matrix34f& getViewMtx(const IUseCamera* cameraHolder, s32 cameraNum);
void calcCameraFront(sead::Vector3f* out, const IUseCamera* cameraHolder, s32 cameraNum);
}  // namespace al
