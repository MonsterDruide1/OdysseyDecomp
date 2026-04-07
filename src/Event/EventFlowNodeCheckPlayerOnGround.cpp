#include "Event/EventFlowNodeCheckPlayerOnGround.h"

#include "Library/Event/EventFlowFunction.h"

#include "Util/PlayerUtil.h"

EventFlowNodeCheckPlayerOnGround::EventFlowNodeCheckPlayerOnGround(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeCheckPlayerOnGround::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    al::initEventQuery(this, info);
}

s32 EventFlowNodeCheckPlayerOnGround::getNextId() const {
    if (rs::isPlayerOnGround(getActor()))
        return al::getCaseEventNextId(this, 1);

    return al::getCaseEventNextId(this, 0);
}

void EventFlowNodeCheckPlayerOnGround::start() {
    end();
}
