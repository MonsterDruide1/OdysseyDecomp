#include "Event/EventFlowNodeGetCollectBgmBonus02.h"

#include "Library/Event/EventFlowFunction.h"

#include "Event/EventFlowNodeCheckCompleteCollectBgm.h"
#include "Event/EventFlowNodeCheckFirstTalkCollectBgm.h"
#include "System/CollectBgm.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderWriter.h"

EventFlowNodeGetCollectBgmBonus02::EventFlowNodeGetCollectBgmBonus02(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeGetCollectBgmBonus02::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
}

void EventFlowNodeGetCollectBgmBonus02::start() {
    GameDataHolderWriter writer(getActor());
    const char* bgmName = CollectBgm::cBonusCollectBgmName02;
    const char* situationName = CollectBgm::cBonusCollectBgmSituationName02;

    GameDataFunction::trySetCollectedBgm(writer, bgmName, situationName);
    end();
}

void EventFlowNodeCheckFirstTalkCollectBgm::start() {
    end();
}

void EventFlowNodeCheckCompleteCollectBgm::start() {
    end();
}
