#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeCheckCompleteCollectBgm : public al::EventFlowNode {
public:
    EventFlowNodeCheckCompleteCollectBgm(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
    s32 getNextId() const override;
};

static_assert(sizeof(EventFlowNodeCheckCompleteCollectBgm) == 0x68);
