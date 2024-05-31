#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "math/seadVectorFwd.h"
#include "playerUtil.h"

namespace al {

bool tryGetArg(bool* val, al::ActorInitInfo const& info, char const* key) {
    return info.getPlacementInfo().getPlacementIter().tryGetBoolByKey(val, key);
}


HitSensor* getHitSensor(al::LiveActor const* actor, char const* name) {
    return alActorSensorFunction::getSensorKeeper(actor)->getSensor(name);
}

void setGravity(const al::LiveActor* actor, const sead::Vector3f& gravity) {
    *actor->getPoseKeeper()->getGravityPtr() = gravity;
}

}

al::HitSensorKeeper* alActorSensorFunction::getSensorKeeper(al::LiveActor const* actor) {
    return actor->mHitSensorKeeper;
}
