#pragma once

#include "Library/Event/EventFlowNode.h"

namespace al {
class EventFlowChoiceInfo;
}

class EventFlowNodeSelectChoice : public al::EventFlowNode {
public:
    EventFlowNodeSelectChoice(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
    void control() override;
    s32 getNextId() const override;

private:
    const char16* mMessage = nullptr;
    al::EventFlowChoiceInfo* mChoiceInfo = nullptr;
};

static_assert(sizeof(EventFlowNodeSelectChoice) == 0x78);
