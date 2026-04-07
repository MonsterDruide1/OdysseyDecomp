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

    mCaseNum = GameDataFunction::calcRestHintNum(getActor()) <=
                       (s32)AmiiboFunction::getSearchAmiiboNum(getActor()) ?
                   1 :
                   0;
    end();
}

s32 EventFlowNodeIsEnableSearchAmiibo::getNextId() const {
    return al::getCaseEventNextId(this, mCaseNum);
}
