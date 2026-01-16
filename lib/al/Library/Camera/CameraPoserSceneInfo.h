#pragma once

#include <basis/seadTypes.h>

namespace al {
class AudioDirector;
class AreaObjDirector;
struct CameraFlagCtrl;
class CameraInputHolder;
class CameraTargetCollideInfoHolder;
class CameraTargetHolder;
class CameraRailHolder;
class CameraRequestParamHolder;
class CollisionDirector;
struct SnapShotCameraSceneInfo;

struct CameraPoserSceneInfo {
    CameraPoserSceneInfo();
    void init(AreaObjDirector* areaObj, CollisionDirector* collision, const AudioDirector* audio);
    void registerCameraRailHolder(CameraRailHolder* railHolder);

    f32 sceneFovyDegree = 35.0f;
    AreaObjDirector* areaObjDirector = nullptr;
    CollisionDirector* collisionDirector = nullptr;
    const AudioDirector* audioDirector = nullptr;
    CameraInputHolder* inputHolder = nullptr;
    CameraTargetHolder* targetHolder = nullptr;
    CameraFlagCtrl* flagCtrl = nullptr;
    CameraRequestParamHolder* requestParamHolder = nullptr;
    CameraTargetCollideInfoHolder* targetCollideInfoHolder = nullptr;
    SnapShotCameraSceneInfo* snapShotCameraSceneInfo = nullptr;
    CameraRailHolder** railHolders = nullptr;
    s32 railHolderNum = 0;
};

}  // namespace al
