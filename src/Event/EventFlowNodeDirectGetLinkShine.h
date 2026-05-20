#pragma once

#include "Library/Event/EventFlowNode.h"

class Shine;

class EventFlowNodeDirectGetLinkShine : public al::EventFlowNode {
public:
    EventFlowNodeDirectGetLinkShine(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;

    void exeWaitCameraInterpole();
    void exeWaitGetDemo();

private:
    Shine* mShine = nullptr;
    bool mIsTimeBalloon = false;
};

static_assert(sizeof(EventFlowNodeDirectGetLinkShine) == 0x78);
