#pragma once

#include <basis/seadTypes.h>

namespace al {
class ByamlIter;

struct BgmBpmInfo {
    static BgmBpmInfo* createInfo(const ByamlIter& iter);
    static s32 compareInfo(const BgmBpmInfo* lhs, const BgmBpmInfo* rhs);

    BgmBpmInfo();
    BgmBpmInfo(const BgmBpmInfo&);

    f32 time = 0.0f;
    f32 measureTime = 0.0f;
    f32 bpm = 0.0f;
    f32 measureStartBeat = 0.0f;
};

static_assert(sizeof(BgmBpmInfo) == 0x10);

struct BgmTimeSignatureInfo {
    static BgmTimeSignatureInfo* createInfo(const ByamlIter& iter);
    static s32 compareInfo(const BgmTimeSignatureInfo* lhs, const BgmTimeSignatureInfo* rhs);

    BgmTimeSignatureInfo();
    BgmTimeSignatureInfo(const BgmTimeSignatureInfo&);

    f32 time = 0.0f;
    f32 beat = 0.0f;
    s32 nn = 4;
    s32 dd = 4;
};

static_assert(sizeof(BgmTimeSignatureInfo) == 0x10);

}  // namespace al
