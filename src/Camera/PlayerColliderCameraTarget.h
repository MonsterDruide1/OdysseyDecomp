#pragma once

#include "Library/Camera/ActorCameraTarget.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {
class LiveActor;
}  // namespace al

class IUsePlayerCollision;

class PlayerColliderCameraTarget : public al::ActorCameraTarget {
public:
    PlayerColliderCameraTarget(const al::LiveActor*, const IUsePlayerCollision*);

    bool isCollideGround() const override;

private:
    char filler_28[8];
};

static_assert(sizeof(PlayerColliderCameraTarget) == 0x30);
