#include "Event/EventFlowNodeCheckMoonLockOpened.h"

#include "Library/Event/EventFlowFunction.h"

#include "System/GameDataFunction.h"

EventFlowNodeCheckMoonLockOpened::EventFlowNodeCheckMoonLockOpened(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeCheckMoonLockOpened::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    al::initEventQuery(this, info);
}

s32 EventFlowNodeCheckMoonLockOpened::getNextId() const {
    GameDataHolderAccessor accessor(getActor());

    if (GameDataFunction::isOpenMoonRock(accessor))
        return al::getCaseEventNextId(this, 1);

    return al::getCaseEventNextId(this, 0);
}

void EventFlowNodeCheckMoonLockOpened::start() {
    end();
}
