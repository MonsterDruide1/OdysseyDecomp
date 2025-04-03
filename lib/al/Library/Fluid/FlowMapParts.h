#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class FlowMapCtrl;

class FlowMapParts : public LiveActor {
public:
    FlowMapParts(const char* name);
    void init(const ActorInitInfo& info) override;
    void appear() override;
    void movement() override;
    void calcAnim() override;
    bool receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) override;

private:
    bool mIsStatic = false;
    FlowMapCtrl* mFlowMapCtrl = nullptr;
};
}  // namespace al
