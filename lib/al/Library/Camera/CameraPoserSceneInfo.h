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

    f32 mSceneFovyDegree;
    AreaObjDirector* mAreaObjDirector;
    CollisionDirector* mCollisionDirector;
    const AudioDirector* mAudioDirector;
    CameraInputHolder* mInputHolder;
    CameraTargetHolder* mTargetHolder;
    CameraFlagCtrl* mFlagCtrl;
    CameraRequestParamHolder* mRequestParamHolder;
    CameraTargetCollideInfoHolder* mTargetColliedeInfoHolder;
    SnapShotCameraSceneInfo* mSnapShotCameraSceneInfo;
};

}  // namespace al
