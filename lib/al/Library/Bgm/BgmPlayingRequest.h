#pragma once

#include <basis/seadTypes.h>

namespace al {
struct BgmPlayingRequest {
    const char* bgmName = nullptr;
    const char* _8 = nullptr;
    const char* _10 = nullptr;
    s32 _18 = -1;
    s32 _1c = 0;
    s32 _20;
    s32 _24;
    s32 _28;
    bool _2c = false;
    bool _2d = false;
    bool _2e = false;
    bool _2f = false;
};
}  // namespace al
