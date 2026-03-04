#include "Library/Event/EventFlowNodeActorBaseMovementStart.h"

#include "Library/Event/EventFlowFunction.h"

namespace al {
EventFlowNodeActorBaseMovementStart::EventFlowNodeActorBaseMovementStart(const char* name)
    : EventFlowNode(name) {}

void EventFlowNodeActorBaseMovementStart::init(const EventFlowNodeInitInfo& info) {
    initEventFlowNode(this, info);
}

void EventFlowNodeActorBaseMovementStart::start() {
    EventFlowNode::start();
    restartEventMovement(this);
    end();
}
}  // namespace al
