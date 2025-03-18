#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct ActorInitInfo;
class LiveActor;

class RailMoveMovement : public HostStateBase<LiveActor> {
public:
    RailMoveMovement(LiveActor* host, const ActorInitInfo& info);

    void exeMove();

private:
    enum class MoveType : u32 { Loop, Turn, Ahead };

    f32 mSpeed = 10.0f;
    MoveType mMoveType = MoveType::Loop;
};

static_assert(sizeof(RailMoveMovement) == 0x28);

RailMoveMovement* tryCreateRailMoveMovement(LiveActor* host, const ActorInitInfo& info);
}  // namespace al
