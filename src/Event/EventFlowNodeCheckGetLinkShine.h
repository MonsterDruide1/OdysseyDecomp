#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeCheckGetLinkShine : public al::EventFlowNode {
public:
    EventFlowNodeCheckGetLinkShine(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    s32 getNextId() const override;
    void start() override;

private:
    s32 mShineIndex = -1;
};

static_assert(sizeof(EventFlowNodeCheckGetLinkShine) == 0x70);
