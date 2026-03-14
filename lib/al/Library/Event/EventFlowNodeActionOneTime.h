#pragma once

#include "Library/Event/EventFlowNode.h"

namespace al {
class EventFlowNodeActionOneTime : public EventFlowNode {
public:
    EventFlowNodeActionOneTime(const char* name);

    void init(const EventFlowNodeInitInfo& info) override;
    void start() override;
    void control() override;

private:
    const char* mActionName = nullptr;
    f32 mActionFrameRate = -1.0f;
};

static_assert(sizeof(EventFlowNodeActionOneTime) == 0x78);
}  // namespace al
