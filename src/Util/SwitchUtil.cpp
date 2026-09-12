#include "Util/SwitchUtil.h"

#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementId.h"
#include "Library/Placement/PlacementInfo.h"

namespace rs {
bool isOnSwitchLinkSave(const al::LiveActor* actor, const al::ActorInitInfo& actorInitInfo) {
    al::PlacementInfo placementInfo;
    if (al::tryGetLinksInfo(&placementInfo, actorInitInfo, "NoDelete_SwitchSave")) {
        al::PlacementId placementId;
        al::getPlacementId(&placementId, placementInfo);
    }
    return false;
}

bool isSaveSwitch(const al::ActorInitInfo& actorInitInfo) {
    return al::isObjectNameSubStr(actorInitInfo, "Save");
}
}  // namespace rs
