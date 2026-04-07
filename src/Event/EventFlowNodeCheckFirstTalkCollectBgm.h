#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeCheckFirstTalkCollectBgm : public al::EventFlowNode {
public:
    EventFlowNodeCheckFirstTalkCollectBgm(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
    s32 getNextId() const override;
};

static_assert(sizeof(EventFlowNodeCheckFirstTalkCollectBgm) == 0x68);
