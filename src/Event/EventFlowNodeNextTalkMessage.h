#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeNextTalkMessage : public al::EventFlowNode {
public:
    EventFlowNodeNextTalkMessage(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void control() override;
};

static_assert(sizeof(EventFlowNodeNextTalkMessage) == 0x68);
