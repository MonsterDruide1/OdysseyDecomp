#pragma once

#include <container/seadBuffer.h>

namespace al {
class PlacementInfo;
class PlacementId;

class ViewIdHolder {
public:
    ViewIdHolder();
    void init(const PlacementInfo& placementInfo);
    const PlacementId& getViewId(s32 idx) const;

    static ViewIdHolder* tryCreate(const PlacementInfo& placementInfo);
    s32 getNumPlacements() const { return mNumPlacements; }

private:
    s32 mNumPlacements = 0;
    PlacementId* mPlacementIds = nullptr;
};
}  // namespace al
