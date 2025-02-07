#include "Library/Placement/PlacementInfo.h"

namespace al {
PlacementInfo::PlacementInfo() = default;

void PlacementInfo::set(const ByamlIter& placement_iter, const ByamlIter& zone_iter) {
    mPlacementIter = placement_iter;
    mZoneIter = zone_iter;
}
}  // namespace al
