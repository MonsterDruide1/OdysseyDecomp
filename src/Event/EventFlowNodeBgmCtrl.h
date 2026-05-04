#pragma once

#include "Library/Event/EventFlowNode.h"

class EventFlowNodeBgmCtrl : public al::EventFlowNode {
public:
    EventFlowNodeBgmCtrl(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
    void control() override;

private:
    const char* mBgmPlayName = nullptr;
    bool mIsBgmPlayStop = false;
    const char* mBgmSituationName = nullptr;
    bool mIsBgmSituationEnd = false;
    s32 mStartDelayFrameNum = 0;
    s32 mDelayFrameNum = 0;
};

static_assert(sizeof(EventFlowNodeBgmCtrl) == 0x90);
