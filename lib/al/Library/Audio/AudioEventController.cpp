#include "Library/Audio/AudioEventController.h"

#include "Library/Yaml/ByamlIter.h"

namespace al {

AudioAddonSoundArchiveInfo::AudioAddonSoundArchiveInfo() = default;

AudioAddonSoundArchiveInfo* AudioAddonSoundArchiveInfo::createInfo(const ByamlIter& iter) {
    AudioAddonSoundArchiveInfo* info = new AudioAddonSoundArchiveInfo();

    info->name = nullptr;
    if (iter.tryGetStringByKey(&info->name, "Name"))
        return info;

    return nullptr;
}

s32 AudioAddonSoundArchiveInfo::compareInfo(const AudioAddonSoundArchiveInfo* lhs,
                                            const AudioAddonSoundArchiveInfo* rhs) {
    return strcmp(lhs->name, rhs->name);
}

}  // namespace al
