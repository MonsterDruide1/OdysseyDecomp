#include "Event/EventFlowNodeCheckLink.h"

#include "Library/Event/EventFlowFunction.h"
#include "Library/Event/EventFlowUtil.h"
#include "Library/Placement/PlacementFunction.h"

EventFlowNodeCheckLink::EventFlowNodeCheckLink(const char* name)
    : al::EventFlowNode(name), mIsExistShineActorLink(false) {}

void EventFlowNodeCheckLink::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    al::initEventQuery(this, info);

    if (al::isNodeName(this, "CheckLinkShine")) {
        const al::ActorInitInfo* actorInitInfo = al::getActorInitInfo(info);
        mIsExistShineActorLink = al::isExistLinkChild(*actorInitInfo, "ShineActor", 0);
    }
}

s32 EventFlowNodeCheckLink::getNextId() const {
    return al::getCaseEventNextId(this, mIsExistShineActorLink);
}

void EventFlowNodeCheckLink::start() {
    end();
}
