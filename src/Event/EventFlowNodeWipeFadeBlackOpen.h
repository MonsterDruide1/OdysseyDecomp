#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeWipeFadeBlackOpen : public al::EventFlowNode {
public:
    EventFlowNodeWipeFadeBlackOpen(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void control() override;

private:
    s32 mFadeStep = -1;
};

static_assert(sizeof(EventFlowNodeWipeFadeBlackOpen) == 0x70);
