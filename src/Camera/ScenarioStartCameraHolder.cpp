#include "Camera/ScenarioStartCameraHolder.h"

#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Scene/SceneUtil.h"

#include "Camera/ScenarioStartCamera.h"

namespace al {
ScenarioStartCamera* createPlacementActorFromFactory(const ActorInitInfo& actorInitInfo,
                                                     const PlacementInfo* placementInfo);
}  // namespace al

ScenarioStartCameraHolder::ScenarioStartCameraHolder() = default;

void ScenarioStartCameraHolder::init(const al::Scene* scene,
                                     const al::ActorInitInfo& actorInitInfo) {
    s32 stageInfoMapNum = al::getStageInfoMapNum(scene);
    if (stageInfoMapNum < 1)
        return;

    for (s32 i = 0; i < stageInfoMapNum; i++) {
        const al::StageInfo* stageInfo = al::getStageInfoMap(scene, i);
        al::PlacementInfo placementInfo;
        s32 count = 0;
        al::tryGetPlacementInfoAndCount(&placementInfo, &count, stageInfo,
                                        "ScenarioStartCameraList");

        if (count < 1)
            continue;

        for (s32 j = 0; j < count; j++) {
            al::PlacementInfo entryPlacementInfo;
            al::getPlacementInfoByIndex(&entryPlacementInfo, placementInfo, j);
            mScenarioStartCameras.pushBack(
                al::createPlacementActorFromFactory(actorInitInfo, &entryPlacementInfo));
        }
    }
}

bool ScenarioStartCameraHolder::isExistEnableNextScenarioStartCamera() const {
    s32 count = mScenarioStartCameras.size();
    if (count < 1)
        return false;

    for (s32 i = 0; i < count; i++) {
        ScenarioStartCamera* camera = mScenarioStartCameras[i];
        if (camera->isEnableStart())
            return true;
        count = mScenarioStartCameras.size();
    }

    return false;
}

bool ScenarioStartCameraHolder::tryStartNextScenarioStartCamera() {
    s32 count = mScenarioStartCameras.size();
    if (count < 1)
        return false;

    for (s32 i = 0; i < count; i++) {
        ScenarioStartCamera* camera = mScenarioStartCameras[i];
        if (camera->isEnableStart()) {
            mCurrentScenarioStartCamera = camera;
            camera->appear();
            return true;
        }
        count = mScenarioStartCameras.size();
    }

    return false;
}

void ScenarioStartCameraHolder::startNextScenarioStartCamera() {
    s32 count = mScenarioStartCameras.size();
    if (count < 1)
        return;

    for (s32 i = 0; i < count; i++) {
        ScenarioStartCamera* camera = mScenarioStartCameras[i];
        if (camera->isEnableStart()) {
            mCurrentScenarioStartCamera = camera;
            camera->appear();
            return;
        }
        count = mScenarioStartCameras.size();
    }
}

s32 ScenarioStartCameraHolder::getCurrentScenarioStartCameraMaxPlayStep() const {
    return ScenarioStartCamera::getMaxPlayStep();
}
