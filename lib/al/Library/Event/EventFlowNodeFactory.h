#pragma once

#include "Library/Factory/Factory.h"

namespace al {
class EventFlowNode;

using EventFlowNodeCreatorFunction = EventFlowNode* (*)(const char* eventFlowNodeName);

class EventFlowNodeFactory : public Factory<EventFlowNodeCreatorFunction> {
public:
    EventFlowNodeFactory();  // "イベントフローノード生成"
};
}  // namespace al
