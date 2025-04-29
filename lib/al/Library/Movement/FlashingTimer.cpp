#include "Library/Movement/FlashingTimer.h"

#include <math/seadMathCalcCommon.h>

namespace al {
FlashingTimer::FlashingTimer(s32 param_1, s32 param_2, s32 param_3, s32 param_4)
    : _4(param_1), _8(param_2), _c(param_3), _10(param_4) {}

void FlashingTimer::start(s32 param_1, s32 param_2, s32 param_3, s32 param_4) {
    _0 = param_1;
    _4 = param_1;
    _8 = param_2;
    _c = param_3;
    _10 = param_4;
    _14 = false;
    _15 = false;
}

// void FlashingTimer::update() {}

bool FlashingTimer::isHurryStart() const {
    return _0 < _8;
}

s32 FlashingTimer::getLastTime() const {
    return sead::Mathi::clampMin(_0, 0);
}
}  // namespace al
