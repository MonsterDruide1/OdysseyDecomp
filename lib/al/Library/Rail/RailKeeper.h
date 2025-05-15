#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"
#include "Library/Rail/IUseRail.h"

namespace al {
class RailRider;
class PlacementInfo;

class RailKeeper : public HioNode, public IUseRail {
public:
    RailKeeper(const PlacementInfo& info);

    RailRider* getRailRider() const override { return mRailRider; }

private:
    RailRider* mRailRider = nullptr;
};

RailKeeper* createRailKeeper(const PlacementInfo& info, const char* linkName);
RailKeeper* tryCreateRailKeeper(const PlacementInfo& info, const char* linkName);
RailKeeper* createRailKeeperIndex(const PlacementInfo& info, s32 linkIndex, const char* linkName);
}  // namespace al
