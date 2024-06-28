#include "Library/Obj/CollisionObj.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorDrawFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/Resource/Resource.h"

namespace al {
CollisionObj::CollisionObj(const ActorInitInfo& info, Resource* res, const char* collisionFileName,
                           HitSensor* hitSensor, const sead::Matrix34f* joinMtx, const char* suffix)
    : LiveActor("コリジョンオブジェ") {
    initActorSceneInfo(this, info);
    initActorPoseTRSV(this);
    initActorCollisionWithResource(this, res, collisionFileName, hitSensor, joinMtx, suffix);
    initActorClipping(this, info);
    invalidateClipping(this);
    initExecutorCollisionMapObjDecorationMovement(this, info);
}
}  // namespace al
