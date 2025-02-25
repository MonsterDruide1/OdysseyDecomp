#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
struct PadDataPack {
    s32 mTrig = 0;
    s32 mHold = 0;
    sead::Vector2f mLeftStick = sead::Vector2f::zero;
    sead::Vector2f mPointer = sead::Vector2f::zero;
};
}  // namespace al
