#pragma once

#include <basis/seadTypes.h>

namespace al {
class ByamlIter;

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

    AudioInfoListWithParts<AudioAddonSoundArchiveInfo>* info = nullptr;
};

static_assert(sizeof(AudioSoundArchiveInfo) == 0x8);
}  // namespace al
