#pragma once

#include "Library/Movement/MoveType.h"
#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct ActorInitInfo;

class RailMoveMovement : public HostStateBase<LiveActor> {
public:
    RailMoveMovement(LiveActor* host, const ActorInitInfo& info);

    void exeMove();

private:
    f32 mSpeed = 10.0f;
    MoveType mMoveType = MoveType::Loop;
};

static_assert(sizeof(RailMoveMovement) == 0x28);

RailMoveMovement* tryCreateRailMoveMovement(LiveActor* host, const ActorInitInfo& info);
}  // namespace al
