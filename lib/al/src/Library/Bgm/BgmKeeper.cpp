#include "Library/Bgm/BgmKeeper.h"

#include "Library/Audio/System/AudioSystemInfo.h"
#include "Library/Bgm/BgmDirector.h"
#include "Library/Bgm/BgmFunction.h"
#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Resource/Resource.h"
#include "Project/Bgm/BgmInfo.h"

namespace al {

BgmKeeper::BgmKeeper(const AudioSystemInfo* audioInfo, BgmDirector* director, const char* string)
    : mBgmDirector(director) {
    if (string != nullptr) {
        mBgmUserInfo = alBgmFunction::tryFindBgmUserInfo(audioInfo->getBgmDataBase(), string);
    }
}

BgmKeeper* BgmKeeper::create(const AudioSystemInfo* audioInfo, BgmDirector* director,
                             const char* string) {
    BgmKeeper* newBgmKeeper;

    if (audioInfo->getSeDataBase() == nullptr) {
        newBgmKeeper = nullptr;
    } else {
        newBgmKeeper = new BgmKeeper(audioInfo, director, string);
    }

    return newBgmKeeper;
}

void BgmKeeper::update() {}

const char* BgmKeeper::getUserName() const {
    if (mBgmUserInfo == nullptr) {
        return nullptr;
    }
    return mBgmUserInfo->mName;
}
}  // namespace al
