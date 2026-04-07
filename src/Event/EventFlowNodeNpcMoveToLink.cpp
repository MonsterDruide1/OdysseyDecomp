#include "Event/EventFlowNodeNpcMoveToLink.h"

#include "Library/Event/EventFlowFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/Placement/PlacementFunction.h"

EventFlowNodeNpcMoveToLink::EventFlowNodeNpcMoveToLink(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeNpcMoveToLink::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);

    const char* linkName = al::getParamIterKeyString(info, "LinkName");
    mIsValidLink = al::tryGetLinksQT(&mQuat, &mTrans, *al::getActorInitInfo(info), linkName);
}

void EventFlowNodeNpcMoveToLink::start() {
    EventFlowNode::start();

    if (mIsValidLink)
        al::resetQuatPosition(getActor(), mQuat, mTrans);

    end();
}
