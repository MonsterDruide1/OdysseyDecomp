#pragma once

#include "Library/Event/IUseEventFlowData.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class HitSensor;

class EventFlowExecutor : public al::HioNode, public al::IUseEventFlowData {
public:
    void attackSensor(HitSensor*, HitSensor*);

    EventFlowDataHolder* getEventFlowDataHolder() const override;
};
}  // namespace al
