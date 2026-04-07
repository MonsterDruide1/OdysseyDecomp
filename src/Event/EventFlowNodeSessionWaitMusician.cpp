#include "Event/EventFlowNodeSessionWaitMusician.h"

#include "Library/Event/EventFlowFunction.h"

#include "Npc/SessionMusicianLocalFunction.h"

EventFlowNodeSessionWaitMusician::EventFlowNodeSessionWaitMusician(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeSessionWaitMusician::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    al::initEventQuery(this, info);
    mCount = al::getParamIterKeyInt(info, "Count");
}

void EventFlowNodeSessionWaitMusician::start() {
    end();
}

s32 EventFlowNodeSessionWaitMusician::getNextId() const {
    if (mCount <= SessionMusicianLocalFunction::getMemberMusicianNum(getActor()))
        return al::getCaseEventNextId(this, 0);

    return al::getCaseEventNextId(this, 1);
}
