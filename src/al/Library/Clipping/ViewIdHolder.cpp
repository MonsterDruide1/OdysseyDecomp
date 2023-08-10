#include "al/Library/Clipping/ViewIdHolder.h"
#include "al/Library/Placement/PlacementFunction.h"
#include "al/Library/Placement/PlacementId.h"
#include "al/Library/Placement/PlacementInfo.h"

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
    if(mNumPlacements >= 1) {
        for (int i = 0; i < mNumPlacements; ++i) {
            PlacementInfo info;
            getLinksInfoByIndex(&info, placementInfo, "ViewGroup", i);
            mPlacementIds[i].init(info);
        }
    }
}
PlacementId& ViewIdHolder::getViewId(int idx) const {
    return mPlacementIds[idx];
}
}
