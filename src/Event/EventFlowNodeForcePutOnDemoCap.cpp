#include "Event/EventFlowNodeForcePutOnDemoCap.h"

#include "Library/Event/EventFlowFunction.h"

#include "Util/PlayerDemoUtil.h"

EventFlowNodeForcePutOnDemoCap::EventFlowNodeForcePutOnDemoCap(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeForcePutOnDemoCap::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
}

void EventFlowNodeForcePutOnDemoCap::start() {
    const al::LiveActor* actor = getActor();

    EventFlowNode::start();
    rs::forcePutOnDemoCap(actor);
    end();
}
