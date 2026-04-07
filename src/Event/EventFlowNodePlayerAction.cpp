#include "Event/EventFlowNodePlayerAction.h"

#include "Library/Event/EventFlowFunction.h"

#include "Util/PlayerDemoUtil.h"

EventFlowNodePlayerAction::EventFlowNodePlayerAction(const char* name) : al::EventFlowNode(name) {}

void EventFlowNodePlayerAction::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    al::tryGetParamIterKeyBool(&mIsWaitAction, info, "IsWaitAction");
    mActionName = al::tryGetParamIterKeyString(info, "ActionName");
    al::tryGetParamIterKeyBool(&mIsDynamicsResetAtStart, info, "IsDynamicsResetAtStart");
}

void EventFlowNodePlayerAction::start() {
    EventFlowNode::start();

    if (mIsDynamicsResetAtStart)
        rs::resetMarioDynamics(getActor());

    rs::startActionDemoPlayer(getActor(), mActionName);

    if (mIsWaitAction)
        return;

    end();
}

void EventFlowNodePlayerAction::control() {
    if (rs::isActionEndDemoPlayer(getActor()))
        end();
}
