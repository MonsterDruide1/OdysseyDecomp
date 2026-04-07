#include "Event/EventFlowNodeCloseTalkMessage.h"

#include "Library/Event/EventFlowFunction.h"

#include "Util/NpcEventFlowUtil.h"

EventFlowNodeCloseTalkMessage::EventFlowNodeCloseTalkMessage(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeCloseTalkMessage::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
}

void EventFlowNodeCloseTalkMessage::control() {
    if (rs::isCloseNpcDemoEventTalkMessage(getActor()))
        end();
    else
        return rs::startCloseNpcDemoEventTalkMessage(getActor());
}
