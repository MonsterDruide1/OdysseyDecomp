#pragma once

namespace al {
class Resource;
class PlacementInfo;
class ByamlIter;

class StageInfo {
public:
    StageInfo(Resource* resource, const ByamlIter& placement_iter, const ByamlIter& zone_iter);
    const ByamlIter& getPlacementIter() const;
    const ByamlIter& getZoneIter() const;

private:
    Resource* mResource;
    PlacementInfo* mPlacementInfo = nullptr;
};

}  // namespace al
