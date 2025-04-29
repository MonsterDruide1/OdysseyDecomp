#pragma once

#include <basis/seadTypes.h>

namespace al {
class FlashingTimer {
public:
    FlashingTimer(s32, s32, s32, s32);

    void start(s32, s32, s32, s32);
    void update();
    bool isHurryStart() const;
    s32 getLastTime() const;

private:
    s32 _0 = -1;
    s32 _4;
    s32 _8;
    s32 _c;
    s32 _10;
    bool _14 = false;
    bool _15 = false;
};

static_assert(sizeof(FlashingTimer) == 0x18);
}  // namespace al
