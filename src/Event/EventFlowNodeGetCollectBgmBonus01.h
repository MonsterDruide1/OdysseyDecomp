#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeGetCollectBgmBonus01 : public al::EventFlowNode {
public:
    EventFlowNodeGetCollectBgmBonus01(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
};

static_assert(sizeof(EventFlowNodeGetCollectBgmBonus01) == 0x68);
