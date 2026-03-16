#pragma once

#include "Library/Event/EventFlowNode.h"

namespace al {
class EventFlowNodeActionLoop : public EventFlowNode {
public:
    EventFlowNodeActionLoop(const char* name);

    void init(const EventFlowNodeInitInfo& info) override;
    void start() override;
    void control() override;

private:
    const char* mActionName = nullptr;
    s32 mStep = 0;
    s32 mMaxStep = 0;
    f32 mActionFrameRate = -1.0f;
    bool mIsStartRandomFrame = false;
};

static_assert(sizeof(EventFlowNodeActionLoop) == 0x80);
}  // namespace al
