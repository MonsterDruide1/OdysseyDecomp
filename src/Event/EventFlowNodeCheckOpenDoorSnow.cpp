#include "Event/EventFlowNodeCheckOpenDoorSnow.h"

#include "Library/Event/EventFlowFunction.h"

#include "Util/NpcEventFlowUtil.h"

EventFlowNodeCheckOpenDoorSnow::EventFlowNodeCheckOpenDoorSnow(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeCheckOpenDoorSnow::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    al::initEventQuery(this, info);
}

s32 EventFlowNodeCheckOpenDoorSnow::getNextId() const {
    if (rs::isEventAfterDoorSnow1(this))
        return al::getCaseEventNextId(this, 1);

    if (rs::isEventAfterDoorSnow2(this))
        return al::getCaseEventNextId(this, 2);

    if (rs::isEventAfterDoorSnow3(this))
        return al::getCaseEventNextId(this, 3);

    if (rs::isEventAfterDoorSnow4(this))
        return al::getCaseEventNextId(this, 4);

    return al::getCaseEventNextId(this, 0);
}

void EventFlowNodeCheckOpenDoorSnow::start() {
    end();
}
