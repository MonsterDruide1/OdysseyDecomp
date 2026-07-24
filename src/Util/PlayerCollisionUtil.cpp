#include "Util/PlayerCollisionUtil.h"

#include "Library/Collision/CollisionPartsKeeperUtil.h"

#include "Player/IUsePlayerHeightCheck.h"

namespace rs {

bool isCollisionCodePress(const al::HitInfo& hitInfo) {
    return al::isFloorCode(&hitInfo, "Press");
}

bool isCollisionCodeGrabCeil(const al::HitInfo& hitInfo) {
    return al::isFloorCode(&hitInfo, "GrabCeil");
}

const char* getRippleGenerateMaterialFlower() {
    return "FlowerForest";
}

bool isAboveGround(const IUsePlayerHeightCheck* playerHeightCheck) {
    return playerHeightCheck->isAboveGround();
}

f32 getGroundHeight(const IUsePlayerHeightCheck* playerHeightCheck) {
    return playerHeightCheck->getGroundHeight();
}

}  // namespace rs
