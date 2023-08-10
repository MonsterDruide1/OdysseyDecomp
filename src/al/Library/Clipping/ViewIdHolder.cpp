#include "al/Library/Clipping/ViewIdHolder.h"
#include "al/Library/Placement/PlacementFunction.h"
#include "al/Library/Placement/PlacementId.h"

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
    mPlacementIds = new PlacementId[calcLinkChildNum(placementInfo, "ViewGroup")];
}
}
