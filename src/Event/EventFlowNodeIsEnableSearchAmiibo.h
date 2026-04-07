#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeIsEnableSearchAmiibo : public al::EventFlowNode {
public:
    EventFlowNodeIsEnableSearchAmiibo(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
    s32 getNextId() const override;

private:
    s32 mCaseNum = 1;
};

static_assert(sizeof(EventFlowNodeIsEnableSearchAmiibo) == 0x70);
