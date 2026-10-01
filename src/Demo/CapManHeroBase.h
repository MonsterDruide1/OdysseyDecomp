#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

class CapManHeroBase : public al::LiveActor {
public:
    CapManHeroBase(const char* name) : al::LiveActor(name) {}

    void makeActorAlive() override;
    void control() override;
};

static_assert(sizeof(CapManHeroBase) == 0x108);
