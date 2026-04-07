#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeFirstTalkEndCollectBgmNpc : public al::EventFlowNode {
public:
    EventFlowNodeFirstTalkEndCollectBgmNpc(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
};

static_assert(sizeof(EventFlowNodeFirstTalkEndCollectBgmNpc) == 0x68);
