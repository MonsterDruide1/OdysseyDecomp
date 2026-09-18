#pragma once

#include "Library/Camera/ActorCameraTarget.h"

class IUsePlayerCollision;

class PlayerColliderCameraTarget : public al::ActorCameraTarget {
public:
    PlayerColliderCameraTarget(const al::LiveActor* actor,
                               const IUsePlayerCollision* playerCollision)
        : ActorCameraTarget(actor, 0.0f, nullptr), mPlayerCollision(playerCollision) {}

    bool isCollideGround() const override;

private:
    const IUsePlayerCollision* mPlayerCollision = nullptr;
};

static_assert(sizeof(PlayerColliderCameraTarget) == 0x30);
