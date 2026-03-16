#include "Library/Event/EventFlowNode.h"

#include "Library/Nerve/NerveKeeper.h"

namespace al {
EventFlowNode::EventFlowNode(const char* name) : mName(name) {}

void EventFlowNode::initNerve(const Nerve* nerve, s32 maxStates) {
    mNerveKeeper = new NerveKeeper(this, nerve, maxStates);
}

void EventFlowNode::execute() {
    if (mNerveKeeper)
        mNerveKeeper->update();

    control();
}

void EventFlowNode::init(const EventFlowNodeInitInfo& initInfo) {}
}  // namespace al
