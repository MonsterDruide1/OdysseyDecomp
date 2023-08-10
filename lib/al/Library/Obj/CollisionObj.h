#pragma once

#include "al/Library/LiveActor/LiveActor.h"
#include "al/Library/Resource/Resource.h"

namespace al {

class CollisionObj : public LiveActor {
public:
    CollisionObj(const ActorInitInfo&, Resource*, const char*, HitSensor*, const sead::Matrix34f*,
                 const char*);
};
static_assert(sizeof(CollisionObj) == 0x108, "CollisionObj Size");

CollisionObj* createCollisionObj(const LiveActor*, const ActorInitInfo&, const char*, HitSensor*,
                                 const char*, const char*);
CollisionObj* createCollisionObjMtx(const LiveActor*, const ActorInitInfo&, const char*, HitSensor*,
                                    const sead::Matrix34f*, const char*);
}  // namespace al
