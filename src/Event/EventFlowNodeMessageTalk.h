#pragma once

#include <basis/seadTypes.h>

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeMessageTalk : public al::EventFlowNode {
public:
    EventFlowNodeMessageTalk(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
    void control() override;
    void end() override;

private:
    const char16* mMessage = nullptr;
    const char* mDemoActionName = nullptr;
    const char* mStartActionName = nullptr;
    f32 mStartActionFrameRate = 1.0f;
    const char* mLoopActionName = nullptr;
    f32 mLoopActionFrameRate = 1.0f;
    bool mIsNeedOpenMessage = false;
    bool mIsSystemMessage = false;
    bool mIsInvalidIconA = false;
    s32 mIconAppearDelayStep = -1;
};

static_assert(sizeof(EventFlowNodeMessageTalk) == 0xA0);
