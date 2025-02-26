#pragma once

#include <basis/seadTypes.h>
#include <nn/nfp/nfp_types.h>

namespace al {
struct NfpCharacterId {
    s16 gameId;
    s16 characterId;
    s16 characterVariant;
};

struct NfpFormatVersion {
    u32 minor : 9;
    u32 major : 13;
};

struct NfpInfo {
    nn::nfp::TagInfo tagInfo{};
    nn::nfp::ModelInfo modelInfo{};
    NfpFormatVersion formatVersion;
    u16 _9c = 0;
    u16 _9e;
    char _a0[0x9c]{};

    // Unknown size
    char16 nickName[nn::nfp::AmiiboNameLength + 1]{};

    char _18e[0x8e]{};
    bool isNormalNfc = false;
    bool isAmiibo = false;
    bool isFormatVersionSet = false;
    bool isNeedRegister = false;
    bool isNeedFormat = false;
    bool isNeedRestore = false;

    NfpInfo() {}
};

static_assert(sizeof(NfpInfo) == 0x1e8, "NfpInfo");

}  // namespace al
