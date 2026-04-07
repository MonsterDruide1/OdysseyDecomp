#include "Event/EventFlowNodeGetAmiiboNotSearchHintNum.h"

#include "Library/Event/EventFlowFunction.h"

#include "Amiibo/AmiiboNpcDirector.h"
#include "System/GameDataFunction.h"

EventFlowNodeGetAmiiboNotSearchHintNum::EventFlowNodeGetAmiiboNotSearchHintNum(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeGetAmiiboNotSearchHintNum::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    al::initEventQuery(this, info);
}

void EventFlowNodeGetAmiiboNotSearchHintNum::start() {
    EventFlowNode::start();

    GameDataHolderAccessor accessor(getActor());
    s32 restHintNum = GameDataFunction::calcRestHintNum(accessor);
    s32 amiiboNotSearchHintNum = restHintNum - (s32)AmiiboFunction::getSearchAmiiboNum(getActor());

    if (amiiboNotSearchHintNum <= 0)
        mAmiiboNotSearchHintNum = 0;
    else if (amiiboNotSearchHintNum == 1)
        mAmiiboNotSearchHintNum = 1;
    else if (amiiboNotSearchHintNum == 2)
        mAmiiboNotSearchHintNum = 2;
    else
        mAmiiboNotSearchHintNum = 3;

    end();
}

s32 EventFlowNodeGetAmiiboNotSearchHintNum::getNextId() const {
    return al::getCaseEventNextId(this, mAmiiboNotSearchHintNum);
}
