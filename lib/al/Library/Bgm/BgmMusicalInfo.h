#pragma once

#include <basis/seadTypes.h>

namespace al {
template <typename T>
class AudioInfoListWithParts;
class ByamlIter;
struct BgmRhythmInfo;
struct BgmChordInfo;
struct BgmBpmInfo;
struct BgmTimeSignatureInfo;

struct BgmMusicalInfo {
    static BgmMusicalInfo* createInfo(const ByamlIter& iter, const char* name);
    static s32 compareInfo(const BgmMusicalInfo* lhs, const BgmMusicalInfo* rhs);

    BgmMusicalInfo();
    BgmMusicalInfo(const BgmMusicalInfo&);

    const char* name = nullptr;
    AudioInfoListWithParts<BgmRhythmInfo>* rhythmInfoList = nullptr;
    AudioInfoListWithParts<BgmChordInfo>* chordInfoList = nullptr;
    AudioInfoListWithParts<BgmBpmInfo>* bpmInfoList = nullptr;
    AudioInfoListWithParts<BgmTimeSignatureInfo>* timeSignatureInfoList = nullptr;
    f32 beatStartOffsetTime = 0.0f;
};

static_assert(sizeof(BgmMusicalInfo) == 0x30);

}  // namespace al
