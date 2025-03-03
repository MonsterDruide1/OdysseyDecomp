#pragma once

namespace al {
class EventFlowNode;

template <typename T>
EventFlowNode* createEventFlowNode(const char* eventFlowNodeName) {
    return new T(eventFlowNodeName);
}
}  // namespace al
