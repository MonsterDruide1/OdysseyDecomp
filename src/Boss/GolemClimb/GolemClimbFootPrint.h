#pragma once

#include "Library/LiveActor/LiveActor.h"

class GolemClimbFootPrint : public al::LiveActor {
public:
    GolemClimbFootPrint();

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void control() override;
};
