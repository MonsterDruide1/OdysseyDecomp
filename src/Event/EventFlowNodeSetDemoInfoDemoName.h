#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeSetDemoInfoDemoName : public al::EventFlowNode {
public:
    EventFlowNodeSetDemoInfoDemoName(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;

private:
    const char* mDemoName = nullptr;
};

static_assert(sizeof(EventFlowNodeSetDemoInfoDemoName) == 0x70);
