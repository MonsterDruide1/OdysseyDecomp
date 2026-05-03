#pragma once

#include <math/seadMatrix.h>

#include "Camera/PlayerColliderCameraTarget.h"

class IUsePlayerCollision;

namespace al {
class LiveActor;
}

class PlayerColliderMatrixCameraTarget : public PlayerColliderCameraTarget {
public:
    PlayerColliderMatrixCameraTarget(const al::LiveActor* actor,
                                     const IUsePlayerCollision* playerCollision,
                                     const sead::Matrix34f* matrix);

    void calcTrans(sead::Vector3f* trans) const override;
    void calcSide(sead::Vector3f* side) const override;
    void calcUp(sead::Vector3f* up) const override;
    void calcFront(sead::Vector3f* front) const override;
    void calcVelocity(sead::Vector3f* velocity) const override;

private:
    const sead::Matrix34f* mMatrix = nullptr;
};

static_assert(sizeof(PlayerColliderMatrixCameraTarget) == 0x38);
