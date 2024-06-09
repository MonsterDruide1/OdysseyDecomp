#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
}
class IUsePlayerCollision;
class IUsePlayerHeightCheck;
class PlayerConst;

namespace rs {

f32 getGroundHeight(const IUsePlayerHeightCheck*);
const sead::Vector3f& getCollidedWallNormal(const IUsePlayerCollision*);
bool isCollidedGround(const IUsePlayerCollision*);
bool isCollidedGroundRunAngle(const al::LiveActor*, const IUsePlayerCollision*, const PlayerConst*);

}  // namespace rs
