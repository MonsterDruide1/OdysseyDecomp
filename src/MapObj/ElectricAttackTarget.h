#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class MtxConnector;
}

class ElectricAttackTarget : public al::LiveActor {
public:
    ElectricAttackTarget(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void appearBySwitch();
    void killBySwitch();
    void initAfterPlacement() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void exeWait();

private:
    al::MtxConnector* mMtxConnector = nullptr;
    f32 mRailMoveSpeed = 60.0f;
};

static_assert(sizeof(ElectricAttackTarget) == 0x118);
