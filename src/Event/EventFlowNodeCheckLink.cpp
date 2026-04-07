#include "Event/EventFlowNodeCheckLink.h"

#include "Library/Event/EventFlowFunction.h"
#include "Library/Event/EventFlowUtil.h"
#include "Library/Placement/PlacementFunction.h"

EventFlowNodeCheckLink::EventFlowNodeCheckLink(const char* name) : al::EventFlowNode(name) {}

void EventFlowNodeCheckLink::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    al::initEventQuery(this, info);

    if (!al::isNodeName(this, "CheckLinkShine"))
        return;
    mIsExistShineActorLink = al::isExistLinkChild(*al::getActorInitInfo(info), "ShineActor", 0);
}

s32 EventFlowNodeCheckLink::getNextId() const {
    return al::getCaseEventNextId(this, mIsExistShineActorLink ? 1 : 0);
}

void EventFlowNodeCheckLink::start() {
    end();
}
