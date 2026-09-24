#pragma once

#include <basis/seadTypes.h>

class GamePadPlayStyleInfo {
public:
    s32 mPadCount;
    s32 mPlayerOneStyle;
    s32 mPlayerTwoStyle;
};

static_assert(sizeof(GamePadPlayStyleInfo) == 0xc);
