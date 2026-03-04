#include "Library/Audio/AudioEventController.h"

#include "Library/Audio/AudioInfo.h"
#include "Library/Yaml/ByamlIter.h"

namespace al {

AudioAddonSoundArchiveInfo::AudioAddonSoundArchiveInfo() = default;

AudioAddonSoundArchiveInfo* AudioAddonSoundArchiveInfo::createInfo(const ByamlIter& iter) {
    AudioAddonSoundArchiveInfo* info = new AudioAddonSoundArchiveInfo();

    if (!iter.tryGetStringByKey(&info->name, "Name"))
        return nullptr;

    return info;
}

s32 AudioAddonSoundArchiveInfo::compareInfo(const AudioAddonSoundArchiveInfo* lhs,
                                            const AudioAddonSoundArchiveInfo* rhs) {
    return strcmp(lhs->name, rhs->name);
}

AudioSoundArchiveInfo::AudioSoundArchiveInfo() = default;

AudioSoundArchiveInfo* AudioSoundArchiveInfo::createInfo(const ByamlIter& iter) {
    AudioSoundArchiveInfo* addon = new AudioSoundArchiveInfo();

    ByamlIter addonIter;
    if (!iter.tryGetIterByKey(&addonIter, "AddonSoundArchiveInfoList"))
        addon->info = nullptr;

    // BUG: addonIter can be invalid. It should have returned early on the previous check
    addon->info = createAudioInfoList<AudioAddonSoundArchiveInfo>(addonIter, 0);
    return addon;
}

}  // namespace al
