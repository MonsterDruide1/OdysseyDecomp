#pragma once

#include <basis/seadTypes.h>

class HackCapTrigger {
public:
    HackCapTrigger();

private:
    s32 _0;
    s32 _4;
};

static_assert(sizeof(HackCapTrigger) == 0x8);
