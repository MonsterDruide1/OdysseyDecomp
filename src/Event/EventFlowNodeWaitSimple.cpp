#include "Event/EventFlowNodeWaitSimple.h"

#include "Library/Event/EventFlowFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(EventFlowNodeWaitSimple, Wait);
NERVES_MAKE_NOSTRUCT(EventFlowNodeWaitSimple, Wait);
}  // namespace

EventFlowNodeWaitSimple::EventFlowNodeWaitSimple(const char* name) : al::EventFlowNode(name) {}

void EventFlowNodeWaitSimple::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    initNerve(&Wait, 0);
    al::tryGetParamIterKeyInt(&mWaitFrame, info, "WaitFrame");
}

void EventFlowNodeWaitSimple::start() {
    EventFlowNode::start();

    if (mWaitFrame <= 0)
        end();
    else
        al::setNerve(this, &Wait);
}

void EventFlowNodeWaitSimple::exeWait() {
    if (al::isGreaterEqualStep(this, mWaitFrame))
        end();
}
