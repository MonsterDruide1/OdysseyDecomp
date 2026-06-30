#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeDemoResetPlayerDynamics : public al::EventFlowNode {
public:
    EventFlowNodeDemoResetPlayerDynamics(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
    void control() override;

private:
    s32 mCurrentStep = 0;
    s32 mDelayStep = 0;
};

static_assert(sizeof(EventFlowNodeDemoResetPlayerDynamics) == 0x70);
