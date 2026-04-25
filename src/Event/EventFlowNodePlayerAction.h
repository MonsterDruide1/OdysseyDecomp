#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodePlayerAction : public al::EventFlowNode {
public:
    EventFlowNodePlayerAction(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
    void control() override;

private:
    bool mIsWaitAction = false;
    const char* mActionName = nullptr;
    bool mIsDynamicsResetAtStart = false;
};

static_assert(sizeof(EventFlowNodePlayerAction) == 0x80);
