#include "Event/EventFlowNodeDemoResetPlayerDynamics.h"

#include "Library/Event/EventFlowFunction.h"

#include "Util/PlayerDemoUtil.h"

EventFlowNodeDemoResetPlayerDynamics::EventFlowNodeDemoResetPlayerDynamics(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeDemoResetPlayerDynamics::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    mDelayStep = al::getParamIterKeyInt(info, "DelayStep");
}

void EventFlowNodeDemoResetPlayerDynamics::start() {
    EventFlowNode::start();
    mCurrentStep = -1;
}

void EventFlowNodeDemoResetPlayerDynamics::control() {
    mCurrentStep++;

    if (mDelayStep <= mCurrentStep) {
        rs::resetMarioDynamics(getActor());
        end();
    }
}
