#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeSceneWipeClose : public al::EventFlowNode {
public:
    EventFlowNodeSceneWipeClose(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
    void control() override;

private:
    s32 mCurrentDelayStep = 0;
    s32 mDelayStep = 0;
    const char* mSituationName = nullptr;
};

static_assert(sizeof(EventFlowNodeSceneWipeClose) == 0x78);
