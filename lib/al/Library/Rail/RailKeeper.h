#pragma once

#include "Library/HostIO/HioNode.h"
#include "Library/Rail/IUseRail.h"

namespace al {
class RailRider;
class PlacementInfo;

class RailKeeper : public IUseHioNode, public IUseRail {
public:
    RailKeeper(const PlacementInfo&);
    RailRider* getRailRider() const override;

private:
    RailRider* mRailRider = nullptr;
};

RailKeeper* tryCreateRailKeeper(const PlacementInfo& info, const char* linkName);
}  // namespace al
