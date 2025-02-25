#include "Library/Camera/ActorCameraTarget.h"

#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {

ActorCameraTarget::ActorCameraTarget(const LiveActor* actor, f32 yOffset,
                                     const sead::Vector3f* localOffset)
    : mActor(actor), mLocalOffset(localOffset), mYOffset(yOffset) {}

const char* ActorCameraTarget::getTargetName() const {
    return mActor->getName();
}

void ActorCameraTarget::calcTrans(sead::Vector3f* trans) const {
    calcTransLocalOffset(trans, mActor, mLocalOffset ? *mLocalOffset : sead::Vector3f::zero);
    trans->y += mYOffset;
}

void ActorCameraTarget::calcSide(sead::Vector3f* side) const {
    calcSideDir(side, mActor);
}

void ActorCameraTarget::calcUp(sead::Vector3f* up) const {
    calcUpDir(up, mActor);
}

void ActorCameraTarget::calcFront(sead::Vector3f* front) const {
    calcFrontDir(front, mActor);
}

void ActorCameraTarget::calcGravity(sead::Vector3f* gravity) const {
    gravity->set(getGravity(mActor));
}

void ActorCameraTarget::calcVelocity(sead::Vector3f* velocity) const {
    velocity->set(getVelocity(mActor));
}

bool ActorCameraTarget::isCollideGround() const {
    return isExistActorCollider(mActor) && !isNoCollide(mActor) && isOnGround(mActor, 0);
}

bool ActorCameraTarget::isInWater() const {
    return isInWaterArea(mActor);
}

}  // namespace al
