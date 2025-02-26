#pragma once

namespace al {
class EventFlowEventData;

class IEventFlowEventReceiver {
public:
    virtual bool receiveEvent(const EventFlowEventData* event);
};

}  // namespace al
