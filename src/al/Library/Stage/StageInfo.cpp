#include "al/stage/StageInfo.h"

#include "al/actor/Placement.h"

namespace al {

StageInfo::StageInfo(Resource* resource, const ByamlIter& placement_iter,
                     const ByamlIter& zone_iter)
    : mResource(resource) {
    mPlacementInfo = new PlacementInfo();
    mPlacementInfo->set(placement_iter, zone_iter);
}
const ByamlIter& StageInfo::getPlacementIter() const {
    return mPlacementInfo->getPlacementIter();
}
const ByamlIter& StageInfo::getZoneIter() const {
    return mPlacementInfo->getZoneIter();
}

}  // namespace al
