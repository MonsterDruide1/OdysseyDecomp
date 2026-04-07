#include "Event/EventFlowNodeFirstTalkEndCollectBgmNpc.h"

#include "Library/Event/EventFlowFunction.h"

#include "System/GameDataUtil.h"

EventFlowNodeFirstTalkEndCollectBgmNpc::EventFlowNodeFirstTalkEndCollectBgmNpc(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeFirstTalkEndCollectBgmNpc::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
}

void EventFlowNodeFirstTalkEndCollectBgmNpc::start() {
    rs::talkCollectBgmNpc(getActor());
    end();
}
