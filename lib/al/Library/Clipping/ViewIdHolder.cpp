#include "Library/Clipping/ViewIdHolder.h"

#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementId.h"
#include "Library/Placement/PlacementInfo.h"

namespace al {
ViewIdHolder::ViewIdHolder() {}

ViewIdHolder* ViewIdHolder::tryCreate(const PlacementInfo& placementInfo) {
    if (calcLinkChildNum(placementInfo, "ViewGroup") < 1) {
        return nullptr;
    } else {
        ViewIdHolder* holder = new ViewIdHolder();
        holder->init(placementInfo);
        return holder;
    }
}

void ViewIdHolder::init(const PlacementInfo& placementInfo) {
    mNumPlacements = calcLinkChildNum(placementInfo, "ViewGroup");
    mPlacementIds = new PlacementId[mNumPlacements];
    for (s32 i = 0; i < mNumPlacements; ++i) {
        PlacementInfo info;
        getLinksInfoByIndex(&info, placementInfo, "ViewGroup", i);
        mPlacementIds[i].init(info);
    }
}

PlacementId& ViewIdHolder::getViewId(s32 idx) const {
    return mPlacementIds[idx];
}
}  // namespace al
