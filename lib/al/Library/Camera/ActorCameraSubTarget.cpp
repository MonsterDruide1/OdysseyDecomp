#include "Library/Camera/ActorCameraSubTarget.h"

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {

ActorCameraSubTarget::ActorCameraSubTarget(const LiveActor* actor) : mActor(actor) {}

const char* ActorCameraSubTarget::getTargetName() const {
    return mActor->getName();
}

void ActorCameraSubTarget::calcTrans(sead::Vector3f* trans) const {
    trans->set(getTrans(mActor));
    if (mOffset) {
        sead::Vector3f side, up, front;
        calcSide(&side);
        calcUp(&up);
        calcFront(&front);
        *trans += side * mOffset->x + up * mOffset->y + front * mOffset->z;
    }
}

void ActorCameraSubTarget::calcSide(sead::Vector3f* side) const {
    calcSideDir(side, mActor);
}

void ActorCameraSubTarget::calcUp(sead::Vector3f* up) const {
    calcUpDir(up, mActor);
}

void ActorCameraSubTarget::calcFront(sead::Vector3f* front) const {
    calcFrontDir(front, mActor);
}

void ActorCameraSubTarget::calcVelocity(sead::Vector3f* velocity) const {
    velocity->set(getVelocity(mActor));
}

ActorBackAroundCameraSubTarget::ActorBackAroundCameraSubTarget(const LiveActor* actor)
    : ActorCameraSubTarget(actor) {
    mTargetName.format("%s[背後回り込み]", actor->getName());
}

void ActorBackAroundCameraSubTarget::calcTrans(sead::Vector3f* trans) const {
    ActorCameraSubTarget::calcTrans(trans);
    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    calcFrontDir(&front, getActor());
    *trans += front * 200.0f;
}

const char* ActorBackAroundCameraSubTarget::getTargetName() const {
    return mTargetName.cstr();
}

}  // namespace al
