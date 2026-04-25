#include "Event/EventFlowNodeIsTalkAmiiboHelp.h"

#include "Library/Event/EventFlowFunction.h"

#include "System/GameDataFunction.h"

EventFlowNodeIsTalkAmiiboHelp::EventFlowNodeIsTalkAmiiboHelp(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeIsTalkAmiiboHelp::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    al::initEventQuery(this, info);
}

void EventFlowNodeIsTalkAmiiboHelp::start() {
    EventFlowNode::start();

    mCaseNum = !GameDataFunction::isEnableExplainAmiibo(getActor()) ? 1 : 0;
    end();
}

s32 EventFlowNodeIsTalkAmiiboHelp::getNextId() const {
    return al::getCaseEventNextId(this, mCaseNum);
}
