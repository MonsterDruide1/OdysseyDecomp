#pragma once

#include "Library/HostIO/HioNode.h"
#include "Library/Rail/IUseRail.h"

namespace al {
class RailRider;
class PlacementInfo;

class RailKeeper : public HioNode, public IUseRail {
public:
    RailKeeper(const PlacementInfo&);

    RailRider* getRailRider() const override { return mRailRider; }

private:
    RailRider* mRailRider = nullptr;
};

RailKeeper* tryCreateRailKeeper(const PlacementInfo& info, const char* linkName);
}  // namespace al
