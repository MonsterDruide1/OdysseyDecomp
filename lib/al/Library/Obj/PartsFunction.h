#pragma once

#include <math/seadMatrix.h>

namespace al {
class ActorInitInfo;
class CollisionObj;
class HitSensor;
class LiveActor;
class Resource;

CollisionObj* createCollisionObj(const LiveActor* parent, const ActorInitInfo& info,
                                 const char* collisionFileName, HitSensor* hitSensor,
                                 const char* joinMtxName, const char* suffix);
CollisionObj* createCollisionObjMtx(const LiveActor* parent, const ActorInitInfo& info,
                                    Resource* res, const char* collisionFileName,
                                    HitSensor* hitSensor, const sead::Matrix34f* joinMtx,
                                    const char* suffix);
}  // namespace al
