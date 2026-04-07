#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeGetAmiiboNotSearchHintNum : public al::EventFlowNode {
public:
    EventFlowNodeGetAmiiboNotSearchHintNum(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
    s32 getNextId() const override;

private:
    s32 mAmiiboNotSearchHintNum = 0;
};

static_assert(sizeof(EventFlowNodeGetAmiiboNotSearchHintNum) == 0x70);
