#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class Resource;

class CollisionObj : public LiveActor {
public:
    CollisionObj(const ActorInitInfo& info, Resource* res, const char* collisionFileName,
                 HitSensor* hitSensor, const sead::Matrix34f* joinMtx, const char* suffix);
};

static_assert(sizeof(CollisionObj) == 0x108, "CollisionObj Size");
}  // namespace al
