#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeGetCollectBgmBonus02 : public al::EventFlowNode {
public:
    EventFlowNodeGetCollectBgmBonus02(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
};

static_assert(sizeof(EventFlowNodeGetCollectBgmBonus02) == 0x68);
