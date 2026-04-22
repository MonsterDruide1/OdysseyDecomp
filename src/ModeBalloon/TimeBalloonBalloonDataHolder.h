#pragma once

#include <basis/seadTypes.h>

class TimeBalloonBalloonDataHolder {
public:
    s32 getCurrentCount() const { return mCurrentCount; }

private:
    u8 _0[0x10];
    s32 mCurrentCount = 0;
};
