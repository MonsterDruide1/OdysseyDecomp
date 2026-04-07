#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeCheckMoonLockOpened : public al::EventFlowNode {
public:
    EventFlowNodeCheckMoonLockOpened(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    s32 getNextId() const override;
    void start() override;
};

static_assert(sizeof(EventFlowNodeCheckMoonLockOpened) == 0x68);
