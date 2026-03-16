#include "Library/Event/EventFlowNodeActorKill.h"

#include "Library/Event/EventFlowFunction.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {
EventFlowNodeActorKill::EventFlowNodeActorKill(const char* name) : EventFlowNode(name) {}

void EventFlowNodeActorKill::init(const EventFlowNodeInitInfo& info) {
    initEventFlowNode(this, info);
}

void EventFlowNodeActorKill::start() {
    EventFlowNode::start();
    getActor()->kill();
    end();
}
}  // namespace al
