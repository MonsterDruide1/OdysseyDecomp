#pragma once

#include <nn/nfp/nfp_types.h>

namespace al {
struct NfpCharacterId {
    u16 _0;
    u16 _2;
    u16 _4;
};

struct NfpInfo {
    nn::nfp::TagInfo tagInfo{};
    nn::nfp::ModelInfo modelInfo{};
    u16 _98;
    u16 _9a;
    u16 _9c{};
    u16 _9e;
    char _a0[0x140]{};
    bool _1e0{};
    bool isValid{};
    bool _1e2{};
    bool _1e3{};
    bool _1e4{};
    bool _1e5{};

    NfpInfo() {}
};

static_assert(sizeof(NfpInfo) == 0x1e8, "NfpInfo");

}  // namespace al
