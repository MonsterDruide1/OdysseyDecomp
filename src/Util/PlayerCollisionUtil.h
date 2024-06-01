#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}
class IUsePlayerCollision;
class IUsePlayerHeightCheck;
class PlayerConst;

namespace rs {

f32 getGroundHeight(const IUsePlayerHeightCheck*);
bool isCollidedGroundRunAngle(const al::LiveActor*, const IUsePlayerCollision*, const PlayerConst*);

}  // namespace rs
