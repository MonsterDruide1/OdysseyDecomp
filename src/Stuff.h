#pragma once

#include "Library/Collision/IUseCollision.h"
#include "Library/Collision/KTriangle.h"

namespace rs {

bool isCollisionCodePress(al::HitInfo*);
bool calcExistCollisionBorder(const al::IUseCollision*, const sead::Vector3f&, const sead::Vector3f&);

}
