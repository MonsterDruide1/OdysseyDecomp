#pragma once

#include <math/seadVector.h>

class IUsePlayerCollision;
class IUsePlayerHeightCheck;

namespace rs {

f32 getGroundHeight(const IUsePlayerHeightCheck*);
const sead::Vector3f& getCollidedWallNormal(const IUsePlayerCollision*);
bool isCollidedGround(const IUsePlayerCollision*);

}  // namespace rs
