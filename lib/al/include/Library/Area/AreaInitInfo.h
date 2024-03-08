#pragma once

#include "Library/Placement/PlacementInfo.h"

namespace al {
class PlacementInfo;
class StageSwitchDirector;
class SceneObjHolder;

class AreaInitInfo : public al::PlacementInfo {
private:
    al::StageSwitchDirector* mStageSwitchDirector;
    al::SceneObjHolder* mSceneObjHolder;

public:
    AreaInitInfo(){};
    AreaInitInfo(const al::PlacementInfo& placementInfo,
                 al::StageSwitchDirector* stageSwitchDirector, al::SceneObjHolder* sceneObjHolder);
    AreaInitInfo(const al::PlacementInfo& placementInfo, const al::AreaInitInfo& initInfo);

    void set(const al::PlacementInfo& placementInfo, al::StageSwitchDirector* stageSwitchDirector,
             al::SceneObjHolder* sceneObjHolder);

    al::StageSwitchDirector* getStageSwitchDirector() const { return mStageSwitchDirector; }
    al::SceneObjHolder* getSceneObjHolder() const { return mSceneObjHolder; }
};
}  // namespace al
