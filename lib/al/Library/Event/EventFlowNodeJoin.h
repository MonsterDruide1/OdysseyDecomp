#pragma once

#include "Library/Event/EventFlowNode.h"

namespace al {
class EventFlowNodeJoin : public EventFlowNode {
public:
    EventFlowNodeJoin(const char* name) : EventFlowNode(name) {}

    void init(const EventFlowNodeInitInfo& info) override;
};
}  // namespace al
