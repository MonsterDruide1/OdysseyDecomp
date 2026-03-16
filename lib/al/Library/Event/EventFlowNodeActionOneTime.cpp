#include "Library/Event/EventFlowNodeActionOneTime.h"

#include "Library/Event/EventFlowFunction.h"
#include "Library/Event/EventFlowUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"

namespace al {
EventFlowNodeActionOneTime::EventFlowNodeActionOneTime(const char* name) : EventFlowNode(name) {}

void EventFlowNodeActionOneTime::init(const EventFlowNodeInitInfo& info) {
    initEventFlowNode(this, info);
    mActionName = getParamIterKeyString(info, "ActionName");
    tryGetParamIterKeyFloat(&mActionFrameRate, info, "ActionFrameRate");
}

void EventFlowNodeActionOneTime::start() {
    EventFlowNode::start();
    startEventAction(getActor(), this, mActionName);

    if (mActionFrameRate > 0.0f)
        setActionFrameRate(getActor(), mActionFrameRate);
}

void EventFlowNodeActionOneTime::control() {
    if (isActionEnd(getActor()))
        end();
}
}  // namespace al
