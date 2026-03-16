#pragma once

#include "Library/Event/EventFlowNode.h"

namespace al {
class EventFlowNodeActorKill : public EventFlowNode {
public:
    EventFlowNodeActorKill(const char* name);

    void init(const EventFlowNodeInitInfo& info) override;
    void start() override;
};

static_assert(sizeof(EventFlowNodeActorKill) == 0x68);
}  // namespace al
