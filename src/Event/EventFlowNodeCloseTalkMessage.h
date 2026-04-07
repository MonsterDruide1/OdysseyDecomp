#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeCloseTalkMessage : public al::EventFlowNode {
public:
    EventFlowNodeCloseTalkMessage(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void control() override;
};

static_assert(sizeof(EventFlowNodeCloseTalkMessage) == 0x68);
