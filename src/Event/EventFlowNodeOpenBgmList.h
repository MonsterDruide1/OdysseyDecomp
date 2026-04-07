#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeOpenBgmList : public al::EventFlowNode {
public:
    EventFlowNodeOpenBgmList(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
    void control() override;
};

static_assert(sizeof(EventFlowNodeOpenBgmList) == 0x68);
