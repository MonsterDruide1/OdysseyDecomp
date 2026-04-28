#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class BalloonOrderGroup;
class PlacementInfo;

class BalloonOrderGroupHolder {
public:
    BalloonOrderGroupHolder();

    BalloonOrderGroup* tryFindGroup(const PlacementInfo& placementInfo) const;
    void registerGroup(BalloonOrderGroup* group);
    void updateAll(const sead::Vector3f& position);
    void resetInsideTerritoryAll();

private:
    s32 mGroupCount;
    BalloonOrderGroup** mGroups;
};

static_assert(sizeof(BalloonOrderGroupHolder) == 0x10);
}  // namespace al
