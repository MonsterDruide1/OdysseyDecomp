#include "Event/EventFlowNodePopLinkShine.h"

#include "Library/Event/EventFlowFunction.h"

#include "Util/ItemUtil.h"

EventFlowNodePopLinkShine::EventFlowNodePopLinkShine(const char* name) : al::EventFlowNode(name) {}

void EventFlowNodePopLinkShine::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);

    const char* linkName = al::tryGetParamIterKeyString(info, "LinkName");

    if (linkName == nullptr)
        linkName = "ShineActor";

    mShine = rs::initLinkShine(*al::getActorInitInfo(info), linkName, 0);
}

void EventFlowNodePopLinkShine::start() {
    EventFlowNode::start();

    if (mShine == nullptr || mIsAppearShine) {
        end();
        return;
    }

    rs::appearPopupShineWithoutDemo(mShine);
    mIsAppearShine = true;
}

void EventFlowNodePopLinkShine::control() {
    if (rs::isEndAppearShine(mShine))
        end();
}
