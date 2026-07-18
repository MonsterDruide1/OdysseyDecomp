#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeIsTalkAmiiboHelp : public al::EventFlowNode {
public:
    EventFlowNodeIsTalkAmiiboHelp(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
    s32 getNextId() const override;

private:
    s32 mCaseNum = 0;
};

static_assert(sizeof(EventFlowNodeIsTalkAmiiboHelp) == 0x70);
