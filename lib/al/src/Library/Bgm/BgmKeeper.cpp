#include "Library/Bgm/BgmKeeper.h"

#include "Library/Audio/System/AudioSystemInfo.h"
#include "Library/Bgm/BgmDirector.h"
#include "Library/Bgm/BgmFunction.h"
#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Resource/Resource.h"
#include "Project/Bgm/BgmInfo.h"

namespace al {

BgmKeeper::BgmKeeper(const al::AudioSystemInfo* audio_info, al::BgmDirector* director, const char* string) {
    mBgmDirector = director;
    mUserName = nullptr;

    if(string != nullptr) {
        mUserName = alBgmFunction::tryFindBgmUserInfo(audio_info->mBgmDataBase, string);
    }
}
BgmKeeper* BgmKeeper::create(const al::AudioSystemInfo* audio_info, al::BgmDirector* director, const char* string){ 
  BgmKeeper* new_bgmkeeper;

  if (audio_info->mSeDataBase == nullptr) {
    new_bgmkeeper = nullptr;
  } else {
    new_bgmkeeper = new BgmKeeper(audio_info, director, string);
  }

  return new_bgmkeeper;
}
}