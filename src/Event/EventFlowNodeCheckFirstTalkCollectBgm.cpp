#include "Event/EventFlowNodeCheckFirstTalkCollectBgm.h"

#include "Library/Event/EventFlowFunction.h"

#include "System/GameDataUtil.h"

EventFlowNodeCheckFirstTalkCollectBgm::EventFlowNodeCheckFirstTalkCollectBgm(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeCheckFirstTalkCollectBgm::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    al::initEventQuery(this, info);
}

s32 EventFlowNodeCheckFirstTalkCollectBgm::getNextId() const {
    return al::getCaseEventNextId(this, !rs::isTalkCollectBgmNpc(getActor()));
}
