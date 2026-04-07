#include "Event/EventFlowNodeIsEnableSearchAmiibo.h"

#include "Library/Event/EventFlowFunction.h"

#include "Amiibo/AmiiboNpcDirector.h"
#include "System/GameDataFunction.h"

EventFlowNodeIsEnableSearchAmiibo::EventFlowNodeIsEnableSearchAmiibo(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeIsEnableSearchAmiibo::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    al::initEventQuery(this, info);
}

void EventFlowNodeIsEnableSearchAmiibo::start() {
    EventFlowNode::start();

    GameDataHolderAccessor accessor(getActor());
    s32 restHintNum = GameDataFunction::calcRestHintNum(accessor);

    mIsEnableSearchAmiibo = restHintNum <= (s32)AmiiboFunction::getSearchAmiiboNum(getActor());
    end();
}

s32 EventFlowNodeIsEnableSearchAmiibo::getNextId() const {
    return al::getCaseEventNextId(this, mIsEnableSearchAmiibo);
}
