#include "Event/EventFlowNodeGetCollectBgmBonus01.h"

#include "Library/Event/EventFlowFunction.h"

#include "System/CollectBgm.h"
#include "System/GameDataFunction.h"

EventFlowNodeGetCollectBgmBonus01::EventFlowNodeGetCollectBgmBonus01(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeGetCollectBgmBonus01::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
}

void EventFlowNodeGetCollectBgmBonus01::start() {
    GameDataFunction::trySetCollectedBgm(getActor(), CollectBgm::cBonusCollectBgmName01, nullptr);
    end();
}
