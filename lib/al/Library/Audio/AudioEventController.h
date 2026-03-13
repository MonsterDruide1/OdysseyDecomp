#pragma once

#include <basis/seadTypes.h>

namespace al {
class ByamlIter;
class Resource;

template <typename T>
class AudioInfoListWithParts;

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
