#include "Event/EventFlowNodeEnableHint.h"

#include "Library/Event/EventFlowFunction.h"

#include "System/GameDataFunction.h"

EventFlowNodeEnableHint::EventFlowNodeEnableHint(const char* name) : al::EventFlowNode(name) {}

void EventFlowNodeEnableHint::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    mShineIndex =
        GameDataFunction::tryFindLinkedShineIndex(getActor(), *al::getActorInitInfo(info));
}

void EventFlowNodeEnableHint::start() {
    EventFlowNode::start();
    GameDataFunction::enableHintByShineIndex(getActor(), mShineIndex);
    end();
}
