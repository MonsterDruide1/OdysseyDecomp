#pragma once

#include "Library/Event/EventFlowMovement.h"

namespace al {
struct ActorInitInfo;
class LiveActor;
}  // namespace al

class EventActorMovementRailTraffic : public al::EventFlowMovement {
public:
    EventActorMovementRailTraffic(const char* name, al::LiveActor* actor);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;

    void exeMove();
    void exeStopByOtherNpc();
    void exeStopAfter();
};

static_assert(sizeof(EventActorMovementRailTraffic) == 0x30);
