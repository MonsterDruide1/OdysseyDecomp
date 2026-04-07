#include "Event/EventFlowNodeSetDemoInfoDemoName.h"

#include "Library/Event/EventFlowFunction.h"

#include "Util/DemoUtil.h"

EventFlowNodeSetDemoInfoDemoName::EventFlowNodeSetDemoInfoDemoName(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeSetDemoInfoDemoName::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    mDemoName = al::tryGetParamIterKeyString(info, "DemoName");
}

void EventFlowNodeSetDemoInfoDemoName::start() {
    EventFlowNode::start();
    rs::setDemoInfoDemoName(getActor(), mDemoName);
    end();
}
