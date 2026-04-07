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

    s32 amiiboNotSearchHintNum = GameDataFunction::calcRestHintNum(getActor()) -
                                 (s32)AmiiboFunction::getSearchAmiiboNum(getActor());

    if (amiiboNotSearchHintNum <= 0)
        mCaseNum = 0;
    else if (amiiboNotSearchHintNum == 1)
        mCaseNum = 1;
    else if (amiiboNotSearchHintNum == 2)
        mCaseNum = 2;
    else
        mCaseNum = 3;

    end();
}

s32 EventFlowNodeGetAmiiboNotSearchHintNum::getNextId() const {
    return al::getCaseEventNextId(this, mCaseNum);
}
