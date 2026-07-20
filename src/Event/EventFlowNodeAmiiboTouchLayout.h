#pragma once

#include <basis/seadTypes.h>

#include "Library/Event/EventFlowNode.h"

namespace al {
struct NfpInfo;
}

class EventFlowNodeAmiiboTouchLayout : public al::EventFlowNode {
public:
    EventFlowNodeAmiiboTouchLayout(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
    s32 getNextId() const override;

    bool tryCancel();
    void touch(const al::NfpInfo* nfpInfo);

    void exeRequestAppearLayout();
    void exeWaitTouchTrigger();
    void exeCountHold();
    void exeWaitTouchAmiibo();
    void exeIconEnd();
    void exeWaitEndLayout();

private:
    s32 mCaseEventIdx = 4;
};
