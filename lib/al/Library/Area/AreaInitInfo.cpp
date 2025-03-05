#include "Library/Area/AreaInitInfo.h"

namespace al {
AreaInitInfo::AreaInitInfo() : PlacementInfo() {}

AreaInitInfo::AreaInitInfo(const al::PlacementInfo& placementInfo,
                           al::StageSwitchDirector* stageSwitchDirector,
                           al::SceneObjHolder* sceneObjHolder)
    : PlacementInfo(placementInfo), mStageSwitchDirector(stageSwitchDirector),
      mSceneObjHolder(sceneObjHolder) {}

AreaInitInfo::AreaInitInfo(const al::PlacementInfo& placementInfo, const al::AreaInitInfo& initInfo)
    : PlacementInfo(placementInfo), mStageSwitchDirector(initInfo.mStageSwitchDirector),
      mSceneObjHolder(initInfo.mSceneObjHolder) {}

void AreaInitInfo::set(const al::PlacementInfo& placementInfo,
                       al::StageSwitchDirector* stageSwitchDirector,
                       al::SceneObjHolder* sceneObjHolder) {
    //    PlacementInfo::set(placementInfo.getPlacementIter(), placementInfo.getZoneIter());
    setZoneIter(placementInfo.getZoneIter());
    setPlacementIter(placementInfo.getPlacementIter());
    mStageSwitchDirector = stageSwitchDirector;
    mSceneObjHolder = sceneObjHolder;
}
}  // namespace al
