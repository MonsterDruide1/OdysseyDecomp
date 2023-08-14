#pragma once

#include "Library/Placement/PlacementInfo.h"

namespace al {
class StageSwitchDirector;
class SceneObjHolder;

class AreaInitInfo {
public:
    AreaInitInfo();
    AreaInitInfo(const al::PlacementInfo& placementInfo,
                 al::StageSwitchDirector* stageSwitchDirector, al::SceneObjHolder* sceneObjHolder);
    AreaInitInfo(const al::PlacementInfo& placementInfo, const al::AreaInitInfo& initInfo);

    void set(const al::PlacementInfo& placementInfo, al::StageSwitchDirector* stageSwitchDirector,
             al::SceneObjHolder* sceneObjHolder);

    const al::PlacementInfo& getPlacementInfo() const { return mPlacementInfo; }
    al::StageSwitchDirector* getStageSwitchDirector() const { return mStageSwitchDirector; }
    al::SceneObjHolder* getSceneObjHolder() const { return mSceneObjHolder; }

private:
    al::PlacementInfo mPlacementInfo;
    al::StageSwitchDirector* mStageSwitchDirector;
    al::SceneObjHolder* mSceneObjHolder;
};
}  // namespace al
