#pragma once

#include <basis/seadTypes.h>

namespace al {
struct BgmPlayingRequest {
    BgmPlayingRequest(const char* name) : bgmName(name) {}

    BgmPlayingRequest(const char* name, const char* suffix, const char* resourceName, s32 fadeFrame,
                      s32 startFrame, s32 unk20, s32 unk24, s32 unk28, bool unk2c, bool unk2d,
                      bool unk2e, bool unk2f)
        : bgmName(name), _8(suffix), _10(resourceName), _18(fadeFrame), _1c(startFrame), _20(unk20),
          _24(unk24), _28(unk28), _2c(unk2c), _2d(unk2d), _2e(unk2e), _2f(unk2f) {}

    const char* bgmName = nullptr;
    const char* _8 = nullptr;
    const char* _10 = nullptr;
    s32 _18 = -1;
    s32 _1c = 15;
    s32 _20;
    s32 _24;
    s32 _28;
    bool _2c = false;
    bool _2d = false;
    bool _2e = false;
    bool _2f = false;
};
}  // namespace al
