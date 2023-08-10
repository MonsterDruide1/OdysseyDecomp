#include "al/Library/Rail/Rail.h"
#include "al/Library/Rail/RailKeeper.h"
#include "al/Library/Rail/RailRider.h"

namespace al {
RailKeeper::RailKeeper(const PlacementInfo& info) {
    auto* rail = new Rail();
    rail->init(info);
    mRailRider = new RailRider(rail);
}
RailRider* RailKeeper::getRailRider() const {
    return mRailRider;
}
}  // namespace al
