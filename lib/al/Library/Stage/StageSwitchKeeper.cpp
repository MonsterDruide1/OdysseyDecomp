#include "Library/Stage/StageSwitchKeeper.h"

#include "Library/Base/StringUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementId.h"
#include "Library/Placement/PlacementInfo.h"
#include "Project/Stage/StageSwitchAccesser.h"

namespace al {
StageSwitchAccesserList::StageSwitchAccesserList() = default;

StageSwitchAccesserList::StageSwitchAccesserList(const StageSwitchAccesser* accessers)
    : accessers(accessers) {}

StageSwitchKeeper::StageSwitchKeeper() = default;

void StageSwitchKeeper::init(StageSwitchDirector* director, const PlacementInfo& placementInfo) {
    mAccesserSize = calcLinkCountClassName(placementInfo, "StageSwitch");
    mAccessers = new StageSwitchAccesser[mAccesserSize];

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
            mAccessers[validIndex].setUseName(mUseName);
            mAccessers[validIndex].init(director, linkName, placementId);
            validIndex++;
        }
    }
}

StageSwitchAccesser* StageSwitchKeeper::tryGetStageSwitchAccesser(const char* linkName) const {
    for (s32 i = 0; i < mAccesserSize; i++)
        if (isEqualString(linkName, mAccessers[i].getLinkName()))
            return &mAccessers[i];

    return nullptr;
}
}  // namespace al
