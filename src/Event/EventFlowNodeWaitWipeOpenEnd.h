#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeWaitWipeOpenEnd : public al::EventFlowNode {
public:
    EventFlowNodeWaitWipeOpenEnd(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
    void exeWait();
};

static_assert(sizeof(EventFlowNodeWaitWipeOpenEnd) == 0x68);
