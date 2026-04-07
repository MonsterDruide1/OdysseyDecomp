#include "Event/EventFlowNodeEnableHint.h"

#include "Library/Event/EventFlowFunction.h"

#include "System/GameDataFunction.h"

EventFlowNodeEnableHint::EventFlowNodeEnableHint(const char* name) : al::EventFlowNode(name) {}

void EventFlowNodeEnableHint::init(const al::EventFlowNodeInitInfo& info) {
    const al::LiveActor* actor;
    const al::ActorInitInfo* actorInitInfo;

    al::initEventFlowNode(this, info);
    actor = getActor();
    actorInitInfo = al::getActorInitInfo(info);
    mShineIndex = GameDataFunction::tryFindLinkedShineIndex(actor, *actorInitInfo);
}

void EventFlowNodeEnableHint::start() {
    const al::LiveActor* actor = getActor();
    s32 shineIndex = mShineIndex;

    EventFlowNode::start();
    GameDataFunction::enableHintByShineIndex(actor, shineIndex);
    end();
}
