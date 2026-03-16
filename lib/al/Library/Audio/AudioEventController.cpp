#include "Library/Audio/AudioEventController.h"

#include "Library/Audio/AudioInfo.h"
#include "Library/Resource/Resource.h"
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
    AudioSoundArchiveInfo* info = new AudioSoundArchiveInfo();

    ByamlIter addonIter;
    if (!iter.tryGetIterByKey(&addonIter, "AddonSoundArchiveInfoList"))
        info->addonInfo = nullptr;

    // BUG: addonIter can be invalid. It should have returned early on the previous check
    info->addonInfo = createAudioInfoList<AudioAddonSoundArchiveInfo>(addonIter, 0);
    return info;
}

AudioResourceLoadingInfo::AudioResourceLoadingInfo() = default;

AudioResourceLoadingInfo* AudioResourceLoadingInfo::createInfo(const Resource* resource) {
    AudioResourceLoadingInfo* info = new AudioResourceLoadingInfo();

    if (resource->isExistFile("SeSoundArchiveInfo.byml")) {
        ByamlIter iter(static_cast<const u8*>(resource->getOtherFile("SeSoundArchiveInfo.byml")));
        info->seInfo = AudioSoundArchiveInfo::createInfo(iter);
    }

    if (resource->isExistFile("BgmSoundArchiveInfo.byml")) {
        ByamlIter iter(static_cast<const u8*>(resource->getOtherFile("BgmSoundArchiveInfo.byml")));
        info->bgmInfo = AudioSoundArchiveInfo::createInfo(iter);
    }

    return info;
}

}  // namespace al
