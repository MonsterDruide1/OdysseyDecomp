#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeWipeFadeBlackClose : public al::EventFlowNode {
public:
    EventFlowNodeWipeFadeBlackClose(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void control() override;

private:
    s32 mFadeStep = -1;
};

static_assert(sizeof(EventFlowNodeWipeFadeBlackClose) == 0x70);
