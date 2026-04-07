#include "Event/EventFlowNodeCheckCompleteCollectBgm.h"

#include "Library/Event/EventFlowFunction.h"

#include "System/GameDataUtil.h"

EventFlowNodeCheckCompleteCollectBgm::EventFlowNodeCheckCompleteCollectBgm(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeCheckCompleteCollectBgm::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    al::initEventQuery(this, info);
}

s32 EventFlowNodeCheckCompleteCollectBgm::getNextId() const {
    return al::getCaseEventNextId(this, rs::checkGetShineCollectedBgmAll(getActor()) ? 1 : 0);
}
