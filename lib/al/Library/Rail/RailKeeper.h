#pragma once

#include "al/Library/HostIO/HioNode.h"
#include "al/Library/Rail/IUseRail.h"

namespace al {
class RailRider;
class PlacementInfo;

class RailKeeper : public IUseHioNode, public IUseRail {
public:
    RailKeeper(const PlacementInfo&);
    RailRider* getRailRider() const override;

private:
    RailRider* mRailRider;
};
}  // namespace al
