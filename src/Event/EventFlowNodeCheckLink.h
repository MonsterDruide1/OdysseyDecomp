#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeCheckLink : public al::EventFlowNode {
public:
    EventFlowNodeCheckLink(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    s32 getNextId() const override;
    void start() override;

private:
    bool mIsExistShineActorLink;
};

static_assert(sizeof(EventFlowNodeCheckLink) == 0x70);
