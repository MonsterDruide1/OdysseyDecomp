#pragma once

#include "Library/Placement/PlacementInfo.h"

namespace al {
class StageSwitchDirector;
class SceneObjHolder;

class AreaInitInfo {
public:
    AreaInitInfo();
    AreaInitInfo(const PlacementInfo& placementInfo, StageSwitchDirector* stageSwitchDirector,
                 SceneObjHolder* sceneObjHolder);
    AreaInitInfo(const PlacementInfo& placementInfo, const AreaInitInfo& initInfo);

    void set(const PlacementInfo& placementInfo, StageSwitchDirector* stageSwitchDirector,
             SceneObjHolder* sceneObjHolder);

    const PlacementInfo& getPlacementInfo() const { return mPlacementInfo; }
    StageSwitchDirector* getStageSwitchDirector() const { return mStageSwitchDirector; }
    SceneObjHolder* getSceneObjHolder() const { return mSceneObjHolder; }

private:
    PlacementInfo mPlacementInfo;
    StageSwitchDirector* mStageSwitchDirector;
    SceneObjHolder* mSceneObjHolder;
};
}  // namespace al
