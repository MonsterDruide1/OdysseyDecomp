#pragma once

#include <basis/seadTypes.h>

namespace al {
template <typename T>
class AudioInfoListWithParts;
struct BgmChordInfo;

class BgmChordDetector {
public:
    BgmChordDetector();

    void init(const AudioInfoListWithParts<BgmChordInfo>* chordInfoList);
    void update(f32 time);
    void forceUninitialized();

private:
    const AudioInfoListWithParts<BgmChordInfo>* mChordInfoList = nullptr;
    const BgmChordInfo* mCurrentChordInfo = nullptr;
};

static_assert(sizeof(BgmChordDetector) == 0x10);

}  // namespace al
