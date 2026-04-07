#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeCheckLifeUpItem : public al::EventFlowNode {
public:
    EventFlowNodeCheckLifeUpItem(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
    s32 getNextId() const override;
    void control() override;

private:
    bool mIsHitPointNotMaxWithItem = false;
};

static_assert(sizeof(EventFlowNodeCheckLifeUpItem) == 0x70);
