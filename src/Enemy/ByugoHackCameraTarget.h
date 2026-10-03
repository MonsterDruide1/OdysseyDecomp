#pragma once

#include "Library/Camera/ActorCameraTarget.h"

namespace {
class ByugoHackCameraTarget : public al::ActorCameraTarget {
public:
    ByugoHackCameraTarget(const al::LiveActor* actor) : ActorCameraTarget(actor, 200.0f, nullptr) {}

    f32 getRequestDistance() const override { return 2400.0f; }
};

static_assert(sizeof(ByugoHackCameraTarget) == 0x28);
}  // namespace
