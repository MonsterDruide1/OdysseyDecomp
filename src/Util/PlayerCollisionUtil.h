#pragma once

namespace al {
class LiveActor;
}
class IUsePlayerCollision;
class PlayerConst;

namespace rs {

bool isCollidedGroundRunAngle(const al::LiveActor*, const IUsePlayerCollision*, const PlayerConst*);

}
