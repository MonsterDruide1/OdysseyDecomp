#pragma once

namespace al {
class EventFlowDataHolder;

class IUseEventFlowData {
public:
    virtual EventFlowDataHolder* getEventFlowDataHolder() const = 0;
};
}  // namespace al
