#include "Event/EventFlowNodeOpenBgmList.h"

#include "Library/Event/EventFlowFunction.h"

#include "Util/NpcEventFlowUtil.h"

EventFlowNodeOpenBgmList::EventFlowNodeOpenBgmList(const char* name) : al::EventFlowNode(name) {}

void EventFlowNodeOpenBgmList::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
}

void EventFlowNodeOpenBgmList::start() {
    EventFlowNode::start();
    rs::requestEventOpenBgmList(this);
}

void EventFlowNodeOpenBgmList::control() {
    if (rs::checkEndSceneExecuteAndResetRequest(this))
        end();
}
