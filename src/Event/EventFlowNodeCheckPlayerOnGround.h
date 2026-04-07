#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeCheckPlayerOnGround : public al::EventFlowNode {
public:
    EventFlowNodeCheckPlayerOnGround(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    s32 getNextId() const override;
    void start() override;
};

static_assert(sizeof(EventFlowNodeCheckPlayerOnGround) == 0x68);
