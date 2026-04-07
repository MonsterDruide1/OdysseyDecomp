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
    const char16* mMessage;
    const char* mDemoActionName;
    const char* mStartActionName;
    f32 mStartActionFrameRate;
    const char* mLoopActionName;
    f32 mLoopActionFrameRate;

    union {
        struct {
            bool isNeedOpenMessage;
            bool isSystemMessage;
            bool isInvalidIconA;
        };

        u32 mMessageFlags;
    };

    s32 mIconAppearDelayStep;
};

static_assert(sizeof(EventFlowNodeMessageTalk) == 0xA0);
