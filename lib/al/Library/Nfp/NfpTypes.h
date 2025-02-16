#pragma once

#include <nn/nfp/nfp_types.h>

namespace al {
struct NfpCharacterId {
    u16 gameId;
    u16 characterId;
    u16 characterVariant;
};

struct NfpInfo {
    nn::nfp::TagInfo tagInfo{};
    nn::nfp::ModelInfo modelInfo{};
    u16 _98;
    u16 _9a;
    u16 _9c = 0;
    u16 _9e;
    char _a0[0x140]{};
    bool _1e0 = false;
    bool isValid = false;
    bool _1e2 = false;
    bool _1e3 = false;
    bool _1e4 = false;
    bool _1e5 = false;

    NfpInfo() {}
};

static_assert(sizeof(NfpInfo) == 0x1e8, "NfpInfo");

}  // namespace al
