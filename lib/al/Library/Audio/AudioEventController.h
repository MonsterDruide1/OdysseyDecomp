#pragma once

#include <basis/seadTypes.h>

namespace al {
class ByamlIter;

struct AudioAddonSoundArchiveInfo {
    AudioAddonSoundArchiveInfo();

    static AudioAddonSoundArchiveInfo* createInfo(const ByamlIter& iter);
    static s32 compareInfo(const AudioAddonSoundArchiveInfo* lhs,
                           const AudioAddonSoundArchiveInfo* rhs);

    const char* name = nullptr;
};

static_assert(sizeof(AudioAddonSoundArchiveInfo) == 0x8);

}  // namespace al
