#pragma once

#include <basis/seadTypes.h>

namespace al {
class PlacementInfo;
class PlacementId;

class ViewIdHolder {
public:
    ViewIdHolder();
    void init(const PlacementInfo& placementInfo);
    const PlacementId& getViewId(s32 idx) const;

    static ViewIdHolder* tryCreate(const PlacementInfo& placementInfo);

private:
    s32 mNumPlacements = 0;
    PlacementId* mPlacementIds = nullptr;
};
}  // namespace al
