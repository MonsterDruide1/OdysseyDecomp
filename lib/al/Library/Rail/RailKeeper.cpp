#include "Library/Rail/RailKeeper.h"

#include "Library/Rail/Rail.h"
#include "Library/Rail/RailRider.h"

namespace al {
RailKeeper::RailKeeper(const PlacementInfo& info) {
    auto* rail = new Rail();
    rail->init(info);
    mRailRider = new RailRider(rail);
}
}  // namespace al
