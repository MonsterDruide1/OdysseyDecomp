#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeEnableHint : public al::EventFlowNode {
public:
    EventFlowNodeEnableHint(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;

private:
    s32 mShineIndex = -1;
};

static_assert(sizeof(EventFlowNodeEnableHint) == 0x70);
