#include "Project/Collision/ActorCollisionController.h"

#include "Library/LiveActor/ActorCollisionFunction.h"

namespace al {

ActorCollisionController::ActorCollisionController(LiveActor* actor) : mActor(actor) {
    mRadius = getColliderRadius(mActor);
    mOffsetY = getColliderOffsetY(mActor);
}

void ActorCollisionController::setColliderRadius(f32 radius) {
    mDelay = -1;
    al::setColliderRadius(mActor, radius);
}

void ActorCollisionController::setColliderRadiusScale(f32 scale) {
    setColliderRadius(mRadius * scale);
    setColliderOffsetY(mOffsetY * scale);
}

void ActorCollisionController::setColliderOffsetY(f32 offsetY) {
    mDelay = -1;
    al::setColliderOffsetY(mActor, offsetY);
}

void ActorCollisionController::update() {
    if (mDelay <= 0)
        return;

    if (mDelay == 1) {
        resetToOrigin(mDelay);
        return;
    }

    f32 radius = getColliderRadius(mActor);
    f32 offsetY = getColliderOffsetY(mActor);
    f32 rate = 1.0f / mDelay;

    radius += (mRadius - radius) * rate;
    offsetY += (mOffsetY - offsetY) * rate;
    al::setColliderRadius(mActor, radius);
    al::setColliderOffsetY(mActor, offsetY);

    mDelay--;
}

void ActorCollisionController::resetToOrigin(s32 delay) {
    if (delay >= 2) {
        mDelay = delay;
        return;
    }

    al::setColliderRadius(mActor, mRadius);
    al::setColliderOffsetY(mActor, mOffsetY);
    mDelay = 0;
}

}  // namespace al
