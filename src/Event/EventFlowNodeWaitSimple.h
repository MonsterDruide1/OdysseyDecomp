#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeWaitSimple : public al::EventFlowNode {
public:
    EventFlowNodeWaitSimple(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
    void exeWait();

private:
    s32 mWaitFrame = 0;
};

static_assert(sizeof(EventFlowNodeWaitSimple) == 0x70);
