#pragma once

#include "Library/LiveActor/LiveActor.h"

// Unused class
class ElectricBall : public al::LiveActor {
public:
    ElectricBall(const char* name, al::LiveActor*);

    void init(const al::ActorInitInfo& info) override;
    void control() override;

    void setChargeLevel(s32);
    void exeWait();

private:
    al::LiveActor* _108;
    s32 _110;
};

static_assert(sizeof(ElectricBall) == 0x118);
