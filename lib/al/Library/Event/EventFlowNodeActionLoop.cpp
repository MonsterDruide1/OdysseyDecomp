#include "Library/Event/EventFlowNodeActionLoop.h"

#include "Library/Event/EventFlowFunction.h"
#include "Library/Event/EventFlowUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"

namespace al {
EventFlowNodeActionLoop::EventFlowNodeActionLoop(const char* name) : EventFlowNode(name) {}

void EventFlowNodeActionLoop::init(const EventFlowNodeInitInfo& info) {
    initEventFlowNode(this, info);
    mActionName = getParamIterKeyString(info, "ActionName");
    tryGetParamIterKeyBool(&mIsStartRandomFrame, info, "IsStartRandomFrame");
    tryGetParamIterKeyInt(&mMaxStep, info, "MaxStep");
    tryGetParamIterKeyFloat(&mActionFrameRate, info, "ActionFrameRate");
}

void EventFlowNodeActionLoop::start() {
    EventFlowNode::start();
    mStep = 0;
    if (!isPlayingEventAction(getActor(), this, mActionName)) {
        if (mIsStartRandomFrame)
            startEventActionAtRandomFrame(getActor(), this, mActionName);
        else
            startEventAction(getActor(), this, mActionName);
    }

    if (mActionFrameRate > 0.0f)
        setActionFrameRate(getActor(), mActionFrameRate);

    if (mMaxStep <= mStep)
        end();
}

void EventFlowNodeActionLoop::control() {
    mStep++;
    if (mMaxStep <= mStep)
        end();
}
}  // namespace al
