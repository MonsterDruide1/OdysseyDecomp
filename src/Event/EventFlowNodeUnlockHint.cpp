#include "Event/EventFlowNodeUnlockHint.h"

#include "Library/Event/EventFlowFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Layout/MapLayout.h"
#include "System/GameDataFunction.h"
#include "Util/NpcEventFlowUtil.h"

namespace {
NERVE_IMPL(EventFlowNodeUnlockHint, Wait);
NERVE_IMPL(EventFlowNodeUnlockHint, TalkMessageCloseWait);
NERVES_MAKE_NOSTRUCT(EventFlowNodeUnlockHint, Wait, TalkMessageCloseWait);
}  // namespace

EventFlowNodeUnlockHint::EventFlowNodeUnlockHint(const char* name) : al::EventFlowNode(name) {}

void EventFlowNodeUnlockHint::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    initNerve(&Wait, 0);
}

void EventFlowNodeUnlockHint::start() {
    EventFlowNode::start();

    GameDataFunction::unlockHint(getActor());
    rs::appearMapWithHint(getActor());

    if (rs::isCloseNpcDemoEventTalkMessage(getActor()))
        al::setNerve(this, &Wait);
    else
        al::setNerve(this, &TalkMessageCloseWait);
}

void EventFlowNodeUnlockHint::exeTalkMessageCloseWait() {
    if (rs::isCloseNpcDemoEventTalkMessage(getActor()))
        al::setNerve(this, &Wait);
    else
        rs::startCloseNpcDemoEventTalkMessage(getActor());
}

void EventFlowNodeUnlockHint::exeWait() {
    if (rs::isEndMap(getActor()))
        end();
}
