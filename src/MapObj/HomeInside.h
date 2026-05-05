#pragma once

#include "Library/LiveActor/LiveActor.h"

class HomeInside : public al::LiveActor {
public:
    HomeInside(const char* name);

    void init(const al::ActorInitInfo& info) override;
};

static_assert(sizeof(HomeInside) == 0x108);
