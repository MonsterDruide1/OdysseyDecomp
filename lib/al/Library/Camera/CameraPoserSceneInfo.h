#pragma once

#include <basis/seadTypes.h>

namespace al {
class AudioDirector;
class AreaObjDirector;
class CameraFlagCtrl;
class CameraInputHolder;
class CameraTargetCollideInfoHolder;
class CameraTargetHolder;
class CameraRailHolder;
class CameraRequestParamHolder;
class CollisionDirector;
struct SnapShotCameraSceneInfo;

struct CameraPoserSceneInfo {
    void init(AreaObjDirector* areaObj, CollisionDirector* collision, const AudioDirector* audio);
    void registerCameraRailHolder(CameraRailHolder* railHolder);

    f32 sceneFovyDegree;
    AreaObjDirector* areaObjDirector;
    CollisionDirector* collisionDirector;
    const AudioDirector* audioDirector;
    CameraInputHolder* inputHolder;
    CameraTargetHolder* targetHolder;
    CameraFlagCtrl* flagCtrl;
    CameraRequestParamHolder* requestParamHolder;
    CameraTargetCollideInfoHolder* targetColliedeInfoHolder;
    SnapShotCameraSceneInfo* snapShotCameraSceneInfo;
    CameraRailHolder* railHolders;
    s32 railHolderNum;
};

}  // namespace al
