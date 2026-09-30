#include "Library/Bgm/BgmChordDetector.h"

#include "Library/Audio/AudioInfo.h"

#include "Project/Bgm/BgmInfo.h"

namespace al {

BgmChordDetector::BgmChordDetector() = default;

void BgmChordDetector::init(const AudioInfoListWithParts<BgmChordInfo>* chordInfoList) {
    mChordInfoList = chordInfoList;
}

void BgmChordDetector::update(f32 time) {
    f32 offsetTime = time + 0.25f;
    if (offsetTime < 0.0f)
        return;

    const AudioInfoListWithParts<BgmChordInfo>* list = mChordInfoList;
    if (!list)
        return;

    mCurrentChordInfo = nullptr;

    for (s32 i = 0; i < list->getSize(); i++) {
        const BgmChordInfo* info = list->getInfoAt(i);
        if (i > 0 && offsetTime < info->beat) {
            mCurrentChordInfo = list->getInfoAt(i - 1);
            if (mCurrentChordInfo)
                return;
            break;
        }
    }

    mCurrentChordInfo = list->getInfoAt(list->getSize() - 1);
}

void BgmChordDetector::forceUninitialized() {
    mChordInfoList = nullptr;
}

}  // namespace al
