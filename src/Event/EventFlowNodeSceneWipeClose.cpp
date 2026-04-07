#include "Event/EventFlowNodeSceneWipeClose.h"

#include "Library/Event/EventFlowFunction.h"
#include "Library/LiveActor/LiveActor.h"

#include "Scene/WipeHolderRequester.h"

EventFlowNodeSceneWipeClose::EventFlowNodeSceneWipeClose(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeSceneWipeClose::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    al::tryGetParamIterKeyInt(&mDelayStep, info, "DelayStep");
    mSituationName = al::getParamIterKeyString(info, "SituationName");
}

void EventFlowNodeSceneWipeClose::start() {
    EventFlowNode::start();
    mCurrentDelayStep = 0;
}

void EventFlowNodeSceneWipeClose::control() {
    if (mDelayStep <= mCurrentDelayStep) {
        rs::requestWipeClose(getActor(), mSituationName);
        end();
    } else {
        mCurrentDelayStep++;
    }
}
