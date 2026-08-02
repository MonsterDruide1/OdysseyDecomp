#pragma once

#include "Library/LiveActor/LiveActor.h"

class ShopBgmPlayer : public al::LiveActor {
public:
    ShopBgmPlayer(const char* name);
    void init(const al::ActorInitInfo& info) override;
    void makeActorAlive() override;
    void movement() override;
    void exeWaitOnSwitch();
    void exePlay();

private:
    char filler[0x10];
};

static_assert(sizeof(ShopBgmPlayer) == 0x118);
