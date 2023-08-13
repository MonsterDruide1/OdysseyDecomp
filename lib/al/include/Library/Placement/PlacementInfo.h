#pragma once

#include <resource/seadResource.h>
#include "Library/Yaml/ByamlIter.h"

namespace al {
class PlacementInfo {
public:
    PlacementInfo();

    void set(const ByamlIter& placement_iter, const ByamlIter& zone_iter);

    const ByamlIter& getPlacementIter() const { return mPlacementIter; }
    const ByamlIter& getZoneIter() const { return mZoneIter; }

private:
    ByamlIter mPlacementIter;
    ByamlIter mZoneIter;
};
}  // namespace al
