#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class MtxConnector;
}

class ElectricSwitch : public al::LiveActor {
public:
    ElectricSwitch(const char* name);
    void init(const al::ActorInitInfo& initInfo) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void initAfterPlacement() override;
    void control() override;
    void exeWait();
    void exeHitReaction();
    void exeReactionEnd();

private:
    al::MtxConnector* mMtxConnector = nullptr;
};
