#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeForcePutOnDemoCap : public al::EventFlowNode {
public:
    EventFlowNodeForcePutOnDemoCap(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
};

static_assert(sizeof(EventFlowNodeForcePutOnDemoCap) == 0x68);
