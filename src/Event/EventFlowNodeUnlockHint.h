#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeUnlockHint : public al::EventFlowNode {
public:
    EventFlowNodeUnlockHint(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
    void exeTalkMessageCloseWait();
    void exeWait();
};

static_assert(sizeof(EventFlowNodeUnlockHint) == 0x68);
