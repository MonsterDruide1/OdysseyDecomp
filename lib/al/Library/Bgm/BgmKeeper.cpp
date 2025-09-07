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
    if (string )
        mBgmUserInfo = alBgmFunction::tryFindBgmUserInfo(audioInfo->bgmDataBase, string);
}

BgmKeeper* BgmKeeper::create(const AudioSystemInfo* audioInfo, BgmDirector* director,
                             const char* string) {
    if (!audioInfo->seDataBase)
        return nullptr;
    return new BgmKeeper(audioInfo, director, string);
}

void BgmKeeper::update() {}

const char* BgmKeeper::getUserName() const {
    if (!mBgmUserInfo )
        return nullptr;
    return mBgmUserInfo->name;
}
}  // namespace al
