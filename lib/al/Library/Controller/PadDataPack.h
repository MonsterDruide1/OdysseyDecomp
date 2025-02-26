#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
struct PadDataPack {
    s32 trig = 0;
    s32 hold = 0;
    sead::Vector2f leftStick = sead::Vector2f::zero;
    sead::Vector2f pointer = sead::Vector2f::zero;
};
}  // namespace al
