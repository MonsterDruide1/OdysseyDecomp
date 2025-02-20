#pragma once

#include <basis/seadTypes.h>
#include <nn/nfp/nfp_types.h>

namespace al {
struct NfpCharacterId {
    s16 gameId;
    s16 characterId;
    s16 characterVariant;
};

struct NfpInfo {
    nn::nfp::TagInfo tagInfo{};
    nn::nfp::ModelInfo modelInfo{};
    u32 formatVersion;
    u16 _9c = 0;
    u16 _9e;
    char _a0[0x9c]{};
    char nickName[nn::nfp::AmiiboNameLength * 4 + 1]{};
    char _18e[0x7b]{};
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
