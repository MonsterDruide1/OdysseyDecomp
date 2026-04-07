#pragma once

#include "Library/Event/EventFlowNode.h"

class Shine;

class EventFlowNodePopLinkShine : public al::EventFlowNode {
public:
    EventFlowNodePopLinkShine(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
    void control() override;

private:
    Shine* mShine = nullptr;
    bool mIsAppearShine = false;
};

static_assert(sizeof(EventFlowNodePopLinkShine) == 0x78);
