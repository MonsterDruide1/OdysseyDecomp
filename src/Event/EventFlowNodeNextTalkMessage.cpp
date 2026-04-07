#include "Event/EventFlowNodeNextTalkMessage.h"

#include "Library/Event/EventFlowFunction.h"

#include "Util/NpcEventFlowUtil.h"

EventFlowNodeNextTalkMessage::EventFlowNodeNextTalkMessage(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeNextTalkMessage::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
}

void EventFlowNodeNextTalkMessage::control() {
    if (rs::isCloseNpcDemoEventTalkMessage(getActor()))
        end();
    else
        rs::startCloseNpcDemoEventTalkMessage(getActor());
}
