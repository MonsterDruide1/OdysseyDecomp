#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <prim/seadSafeString.h>

#include "Library/Execute/IUseExecutor.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
class Resource;
class PlacementInfo;
class ByamlIter;

class StageInfo {
public:
    StageInfo(Resource* resource, const ByamlIter& placement_iter, const ByamlIter& zone_iter);

    const ByamlIter& getPlacementIter() const;
    const ByamlIter& getZoneIter() const;

private:
    Resource* mResource;
    PlacementInfo* mPlacementInfo = nullptr;
};

class StageResourceList {
public:
    StageResourceList(const char* stageName, s32 scenarioNo, const char* resourceType);

    s32 getStageResourceNum() const;
    StageInfo* getStageInfo(s32 index) const;

private:
    sead::PtrArray<StageInfo> mStageResources;
};

}  // namespace al
