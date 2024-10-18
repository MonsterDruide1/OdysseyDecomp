#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveStateBase.h"

enum class RailMoveMovementMoveType : u32 { Loop, Turn, Ahead };

namespace al {
class RailMoveMovement : public HostStateBase<LiveActor> {
public:
    RailMoveMovement(LiveActor* host, const ActorInitInfo& info);

    void exeMove();

private:
    f32 mSpeed = 10.0f;
    RailMoveMovementMoveType mMoveType = RailMoveMovementMoveType::Loop;
};

static_assert(sizeof(RailMoveMovement) == 0x28);

RailMoveMovement* tryCreateRailMoveMovement(LiveActor* host, const ActorInitInfo& info);
}  // namespace al
