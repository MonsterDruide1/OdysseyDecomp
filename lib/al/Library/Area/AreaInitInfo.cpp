#include "Library/Area/AreaInitInfo.h"

namespace al {
AreaInitInfo::AreaInitInfo() = default;

AreaInitInfo::AreaInitInfo(const PlacementInfo& placementInfo,
                           StageSwitchDirector* stageSwitchDirector, SceneObjHolder* sceneObjHolder)
    : PlacementInfo(placementInfo), mStageSwitchDirector(stageSwitchDirector),
      mSceneObjHolder(sceneObjHolder) {}

AreaInitInfo::AreaInitInfo(const PlacementInfo& placementInfo, const AreaInitInfo& initInfo)
    : PlacementInfo(placementInfo), mStageSwitchDirector(initInfo.mStageSwitchDirector),
      mSceneObjHolder(initInfo.mSceneObjHolder) {}

void AreaInitInfo::set(const PlacementInfo& placementInfo, StageSwitchDirector* stageSwitchDirector,
                       SceneObjHolder* sceneObjHolder) {
    setZoneIter(placementInfo.getZoneIter());
    setPlacementIter(placementInfo.getPlacementIter());
    mStageSwitchDirector = stageSwitchDirector;
    mSceneObjHolder = sceneObjHolder;
}
}  // namespace al
