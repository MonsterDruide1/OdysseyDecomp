#include "Event/EventFlowNodeGetCollectBgmBonus02.h"

#include "Library/Event/EventFlowFunction.h"

#include "System/CollectBgm.h"
#include "System/GameDataFunction.h"

EventFlowNodeGetCollectBgmBonus02::EventFlowNodeGetCollectBgmBonus02(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeGetCollectBgmBonus02::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
}

void EventFlowNodeGetCollectBgmBonus02::start() {
    GameDataFunction::trySetCollectedBgm(getActor(), CollectBgm::cBonusCollectBgmName02,
                                         CollectBgm::cBonusCollectBgmSituationName02);
    end();
}
