#include "Event/EventFlowNodeGetCollectBgmBonus01.h"

#include "Library/Event/EventFlowFunction.h"

#include "System/CollectBgm.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderWriter.h"

EventFlowNodeGetCollectBgmBonus01::EventFlowNodeGetCollectBgmBonus01(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeGetCollectBgmBonus01::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
}

void EventFlowNodeGetCollectBgmBonus01::start() {
    GameDataHolderWriter writer(getActor());
    const char* bgmName = CollectBgm::cBonusCollectBgmName01;

    GameDataFunction::trySetCollectedBgm(writer, bgmName, nullptr);
    end();
}
