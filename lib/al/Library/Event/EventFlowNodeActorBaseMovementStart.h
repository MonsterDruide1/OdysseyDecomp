#pragma once

#include "Library/Event/EventFlowNode.h"

namespace al {
class EventFlowNodeActorBaseMovementStart : public EventFlowNode {
public:
    EventFlowNodeActorBaseMovementStart(const char* name);

    void init(const EventFlowNodeInitInfo& info) override;
    void start() override;
};

static_assert(sizeof(EventFlowNodeActorBaseMovementStart) == 0x68);
}  // namespace al
