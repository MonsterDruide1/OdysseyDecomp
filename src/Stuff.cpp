#include "Stuff.h"

#include "Library/Collision/CollisionUtil.h"

namespace rs {

bool isCollisionCodePress(al::HitInfo* info) {
    return al::isFloorCode(info, "Press");
}

}  // namespace rs
