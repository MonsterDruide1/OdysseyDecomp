#include "Event/EventFlowNodeCheckGetLinkShine.h"

#include "Library/Event/EventFlowFunction.h"
#include "Library/Placement/PlacementFunction.h"

#include "System/GameDataFunction.h"

EventFlowNodeCheckGetLinkShine::EventFlowNodeCheckGetLinkShine(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeCheckGetLinkShine::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    al::initEventQuery(this, info);

    const char* linkName = al::tryGetParamIterKeyString(info, "LinkName");

    if (linkName == nullptr)
        linkName = "ShineActor";

    if (al::isExistLinkChild(*al::getActorInitInfo(info), linkName, 0)) {
        mShineIndex = GameDataFunction::tryFindLinkedShineIndexByLinkName(
            getActor(), *al::getActorInitInfo(info), linkName);
    } else {
        mShineIndex = -1;
    }
}

s32 EventFlowNodeCheckGetLinkShine::getNextId() const {
    GameDataHolderAccessor accessor(getActor());
    s32 shineIndex = mShineIndex;

    if (GameDataFunction::isGotShine(accessor, shineIndex))
        return al::getCaseEventNextId(this, 1);

    return al::getCaseEventNextId(this, 0);
}

void EventFlowNodeCheckGetLinkShine::start() {
    end();
}
