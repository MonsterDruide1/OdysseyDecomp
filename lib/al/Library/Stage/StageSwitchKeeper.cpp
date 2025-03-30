#include "Library/Stage/StageSwitchKeeper.h"

#include "Library/Base/StringUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementId.h"
#include "Library/Placement/PlacementInfo.h"
#include "Project/Stage/StageSwitchAccesser.h"

namespace al {
StageSwitchAccesserList::StageSwitchAccesserList() {}

StageSwitchAccesserList::StageSwitchAccesserList(const StageSwitchAccesser* accessers)
    : stageSwitchAccessers(accessers) {}

StageSwitchKeeper::StageSwitchKeeper() {}

void StageSwitchKeeper::init(StageSwitchDirector* director, const PlacementInfo& placementInfo) {
    s32 stageSwitchCount = calcLinkCountClassName(placementInfo, "StageSwitch");
    mAccesserCapacity = stageSwitchCount;
    mStageSwitchAccessers = new StageSwitchAccesser[stageSwitchCount];

    PlacementInfo links;
    tryGetPlacementInfoByKey(&links, placementInfo, "Links");

    s32 linkCount = getCountPlacementInfo(links);
    for (s32 validIndex = 0, i = 0; i < linkCount; i++) {
        PlacementInfo link;
        const char* linkName = nullptr;
        tryGetPlacementInfoAndKeyNameByIndex(&link, &linkName, links, i);
        PlacementInfo linkData;
        tryGetPlacementInfoByIndex(&linkData, link, 0);
        if (isClassName(linkData, "StageSwitch")) {
            PlacementId placementId;
            tryGetPlacementId(&placementId, linkData);
            mStageSwitchAccessers[validIndex].setUseName(mUseName);
            mStageSwitchAccessers[validIndex].init(director, linkName, placementId);
            validIndex++;
        }
    }
}

StageSwitchAccesser* StageSwitchKeeper::tryGetStageSwitchAccesser(const char* linkName) const {
    for (s32 i = 0; i < mAccesserCapacity; i++)
        if (isEqualString(linkName, mStageSwitchAccessers[i].getLinkName()))
            return &mStageSwitchAccessers[i];

    return nullptr;
}
}  // namespace al
