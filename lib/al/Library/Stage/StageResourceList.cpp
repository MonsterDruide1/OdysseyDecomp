#include "Library/Stage/StageResourceList.h"

#include "Library/Base/StringUtil.h"
#include "Library/Execute/ExecuteTableHolderUpdate.h"
#include "Library/File/FileUtil.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Resource/Resource.h"
#include "Library/Resource/ResourceFunction.h"
#include "Library/Scene/SceneUtil.h"
#include "Library/Stage/StageResourceUtil.h"
#include "Library/Yaml/ByamlIter.h"

namespace al {

StageInfo::StageInfo(Resource* resource, const ByamlIter& placement_iter,
                     const ByamlIter& zone_iter)
    : mResource(resource) {
    mPlacementInfo = new PlacementInfo();
    mPlacementInfo->set(placement_iter, zone_iter);
}

const ByamlIter& StageInfo::getPlacementIter() const {
    return mPlacementInfo->getPlacementIter();
}

const ByamlIter& StageInfo::getZoneIter() const {
    return mPlacementInfo->getZoneIter();
}

StageResourceList::StageResourceList(const char* stageName, s32 scenarioNo,
                                     const char* resourceType) {
    StringTmp<128> archivePath;
    makeStageDataArchivePath(&archivePath, stageName, resourceType);
    bool existsArchive = isExistArchive(archivePath);
    if (!existsArchive && isEqualString(resourceType, "Map"))
        return;

    StringTmp<128> mapArchivePath;
    makeStageDataArchivePath(&mapArchivePath, stageName, "Map");
    Resource* mapResource = findOrCreateResource(mapArchivePath, nullptr);

    PlacementInfo placementInfo;
    ByamlIter mapIter = {mapResource->getByml(StringTmp<256>{"%s%s", stageName, "Map"})};

    s32 bymlScenarioIndex = scenarioNo - 1;
    mapIter.tryGetIterByIndex(&mapIter, bymlScenarioIndex);

    StageInfo info = {mapResource, mapIter, {}};

    s32 numResources = 0;
    if (tryGetPlacementInfo(&placementInfo, &info, "ZoneList"))
        numResources = getCountPlacementInfo(placementInfo);

    mStageResources.allocBuffer(numResources + 1, nullptr);

    if (existsArchive) {
        Resource* resource = findOrCreateResource(archivePath, nullptr);
        const u8* bymlData = tryGetByml(resource, StringTmp<256>{"%s%s", stageName, resourceType});
        if (bymlData) {
            ByamlIter placementIter = {bymlData};
            placementIter.tryGetIterByIndex(&placementIter, bymlScenarioIndex);
            mStageResources.pushBack(new StageInfo(resource, placementIter, {}));
        }
    }

    for (s32 i = 0; i < numResources; ++i) {
        PlacementInfo subPlacementInfo;
        tryGetPlacementInfoByIndex(&subPlacementInfo, placementInfo, i);
        const char* objectName = nullptr;
        getObjectName(&objectName, subPlacementInfo);
        makeStageDataArchivePath(&archivePath, objectName, resourceType);
        if (!isExistArchive(archivePath))
            continue;

        Resource* resource = findOrCreateResource(archivePath, nullptr);
        const u8* bymlData = tryGetByml(resource, StringTmp<256>{"%s%s", objectName, resourceType});
        if (!bymlData)
            continue;

        ByamlIter iter = {bymlData};
        iter.tryGetIterByIndex(&iter, bymlScenarioIndex);
        mStageResources.pushBack(
            new StageInfo(resource, iter, subPlacementInfo.getPlacementIter()));

        sead::Vector3f rotation = sead::Vector3f::ones;
        sead::Vector3f scale = sead::Vector3f::zero;
        tryGetRotate(&rotation, subPlacementInfo);
        tryGetScale(&scale, subPlacementInfo);
    }
}

s32 StageResourceList::getStageResourceNum() const {
    return mStageResources.size();
}

StageInfo* StageResourceList::getStageInfo(s32 index) const {
    return mStageResources[index];
}

}  // namespace al
