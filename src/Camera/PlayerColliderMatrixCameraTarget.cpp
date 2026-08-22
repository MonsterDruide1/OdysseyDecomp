#include "Camera/PlayerColliderMatrixCameraTarget.h"

PlayerColliderMatrixCameraTarget::PlayerColliderMatrixCameraTarget(
    const al::LiveActor* actor, const IUsePlayerCollision* playerCollision,
    const sead::Matrix34f* matrix)
    : PlayerColliderCameraTarget(actor, playerCollision), mMatrix(matrix) {}

void PlayerColliderMatrixCameraTarget::calcTrans(sead::Vector3f* trans) const {
    mMatrix->getTranslation(*trans);
}

void PlayerColliderMatrixCameraTarget::calcSide(sead::Vector3f* side) const {
    mMatrix->getBase(*side, 0);
    side->normalize();
}

void PlayerColliderMatrixCameraTarget::calcUp(sead::Vector3f* up) const {
    mMatrix->getBase(*up, 1);
    up->normalize();
}

void PlayerColliderMatrixCameraTarget::calcFront(sead::Vector3f* front) const {
    mMatrix->getBase(*front, 2);
    front->normalize();
}

void PlayerColliderMatrixCameraTarget::calcVelocity(sead::Vector3f* velocity) const {
    velocity->set(0.0f, 0.0f, 0.0f);
}
