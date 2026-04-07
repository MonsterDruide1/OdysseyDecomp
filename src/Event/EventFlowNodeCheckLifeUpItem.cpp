#include "Event/EventFlowNodeCheckLifeUpItem.h"

#include "Library/Event/EventFlowFunction.h"

#include "System/GameDataFunction.h"

EventFlowNodeCheckLifeUpItem::EventFlowNodeCheckLifeUpItem(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeCheckLifeUpItem::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    al::initEventQuery(this, info);
}

void EventFlowNodeCheckLifeUpItem::start() {
    EventFlowNode::start();

    GameDataHolderAccessor accessor(getActor());
    mIsHitPointNotMaxWithItem = !GameDataFunction::isPlayerHitPointMaxWithItem(accessor);
}

s32 EventFlowNodeCheckLifeUpItem::getNextId() const {
    return al::getCaseEventNextId(this, !mIsHitPointNotMaxWithItem);
}

void EventFlowNodeCheckLifeUpItem::control() {
    end();
}
