#pragma once

#include "Library/Event/EventFlowNode.h"

namespace al {
class EventFlowChoiceInfo;
}

class EventFlowNodeSelectYesNo : public al::EventFlowNode {
public:
    EventFlowNodeSelectYesNo(const char* name);

    void init(const al::EventFlowNodeInitInfo& info) override;
    void start() override;
    void control() override;
    s32 getNextId() const override;

private:
    al::EventFlowChoiceInfo* mChoiceInfo = nullptr;
};

static_assert(sizeof(EventFlowNodeSelectYesNo) == 0x70);
