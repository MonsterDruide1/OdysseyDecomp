#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class ByamlIter;
class Resource;

template <typename T>
class AudioInfoListWithParts;
class AudioEventController;
class BgmPlayObj;
class AudioDirector;
}  // namespace al

namespace alAudioEventControllerFunction {
bool tryRegistBgmPlayObj(al::AudioEventController*, al::BgmPlayObj*);
const char* getBgmPlayNameInCurPosition(al::AudioEventController* audioEventController, bool);
const char* getBgmPlayNameInTargetPosition(al::AudioEventController* audioEventController,
                                           const sead::Vector3f&);
bool tryStartAndStopBgmInCurPosition(al::AudioEventController* audioEventController, bool, bool);
void activateAudioEventCtrl(al::AudioDirector*);
void deactivateAudioEventCtrl(al::AudioDirector*);
void tryActivateOrDeactivateAudioEventCtrl(al::AudioDirector*, bool, s32);
}  // namespace alAudioEventControllerFunction

namespace al {
struct AudioAddonSoundArchiveInfo {
    AudioAddonSoundArchiveInfo();

    static AudioAddonSoundArchiveInfo* createInfo(const ByamlIter& iter);
    static s32 compareInfo(const AudioAddonSoundArchiveInfo* lhs,
                           const AudioAddonSoundArchiveInfo* rhs);

    const char* name = nullptr;
};

static_assert(sizeof(AudioAddonSoundArchiveInfo) == 0x8);

struct AudioSoundArchiveInfo {
    AudioSoundArchiveInfo();

    static AudioSoundArchiveInfo* createInfo(const ByamlIter& iter);

    AudioInfoListWithParts<AudioAddonSoundArchiveInfo>* addonInfo = nullptr;
};

static_assert(sizeof(AudioSoundArchiveInfo) == 0x8);

struct AudioResourceLoadingInfo {
    AudioResourceLoadingInfo();

    static AudioResourceLoadingInfo* createInfo(const Resource* resource);

    AudioSoundArchiveInfo* seInfo = nullptr;
    AudioSoundArchiveInfo* bgmInfo = nullptr;
};

static_assert(sizeof(AudioResourceLoadingInfo) == 0x10);

}  // namespace al
