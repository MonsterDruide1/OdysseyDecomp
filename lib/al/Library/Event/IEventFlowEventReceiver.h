#pragma once

namespace al {
class EventFlowEventData;

class IEventFlowEventReceiver {
public:
    virtual bool receiveEvent(const EventFlowEventData* event) = 0;
};

}  // namespace al
