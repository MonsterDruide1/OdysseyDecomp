#pragma once

#include "Library/LiveActor/LiveActor.h"

class Shine;

class ShineFukankunWatchObj : public al::LiveActor {
public:
    ShineFukankunWatchObj(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;

    void exeWait();

private:
    Shine* mShine = nullptr;
};

static_assert(sizeof(ShineFukankunWatchObj) == 0x110);
