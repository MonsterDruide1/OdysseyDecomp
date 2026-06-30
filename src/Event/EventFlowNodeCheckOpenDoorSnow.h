#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeCheckOpenDoorSnow : public al::EventFlowNode {
public:
    EventFlowNodeCheckOpenDoorSnow(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    s32 getNextId() const override;
    void start() override;
};

static_assert(sizeof(EventFlowNodeCheckOpenDoorSnow) == 0x68);
