#include "Event/EventFlowNodeWaitWipeOpenEnd.h"

#include "Library/Event/EventFlowFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Sequence/GameSequenceInfo.h"

namespace {
NERVE_IMPL(EventFlowNodeWaitWipeOpenEnd, Wait);
NERVES_MAKE_NOSTRUCT(EventFlowNodeWaitWipeOpenEnd, Wait);
}  // namespace

EventFlowNodeWaitWipeOpenEnd::EventFlowNodeWaitWipeOpenEnd(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeWaitWipeOpenEnd::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    initNerve(&Wait, 0);
}

void EventFlowNodeWaitWipeOpenEnd::start() {
    EventFlowNode::start();
    al::setNerve(this, &Wait);
}

void EventFlowNodeWaitWipeOpenEnd::exeWait() {
    if (rs::isWipeOpenEnd(getActor()))
        end();
}
