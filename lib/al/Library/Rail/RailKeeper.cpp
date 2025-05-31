#include "Library/Rail/RailKeeper.h"

#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Rail/Rail.h"
#include "Library/Rail/RailRider.h"

namespace al {
RailKeeper::RailKeeper(const PlacementInfo& info) {
    Rail* rail = new Rail();
    rail->init(info);

    mRailRider = new RailRider(rail);
}

RailKeeper* createRailKeeper(const PlacementInfo& info, const char* linkName) {
    return tryCreateRailKeeper(info, linkName);
}

RailKeeper* tryCreateRailKeeper(const PlacementInfo& info, const char* linkName) {
    PlacementInfo linkInfo;
    if (!tryGetLinksInfo(&linkInfo, info, linkName))
        return nullptr;

    return new RailKeeper(linkInfo);
}

RailKeeper* createRailKeeperIndex(const PlacementInfo& info, s32 linkIndex, const char* linkName) {
    PlacementInfo linkInfo;
    getLinksInfoByIndex(&linkInfo, info, linkName, linkIndex);

    return new RailKeeper(linkInfo);
}
}  // namespace al
