#pragma once

#include "Library/LiveActor/LiveActor.h"

class ElectricBall : public al::LiveActor {
public:
    ElectricBall(const char* name, al::LiveActor* parentActor);

    void init(const al::ActorInitInfo& info) override;
    void control() override;
    void setChargeLevel(s32 chargeLevel);
    void exeWait();

private:
    al::LiveActor* mParentActor = nullptr;
    s32 mChargeLevel = 0;
};

static_assert(sizeof(ElectricBall) == 0x118);
