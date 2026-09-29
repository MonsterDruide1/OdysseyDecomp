#include "Library/Audio/System/AudioResourcePlayerKeeper.h"

#include "Library/Audio/AudioEventController.h"
#include "Library/Audio/AudioInfo.h"
#include "Library/Audio/System/AudioPlayer.h"
#include "Library/Base/StringUtil.h"
#include "Project/Memory/Util.h"

namespace al {

AudioResourcePlayer::AudioResourcePlayer(const char* name, const char* fileName,
                                         AudioSoundArchiveInfo* archiveInfo,
                                         IAudioResourceInfoAccessor* accessor)
    : mName(name), mFileName(fileName) {
    mPlayer = new SeadAudioPlayer(archiveInfo, accessor);
    mAddonSize = archiveInfo && archiveInfo->addonInfo ? archiveInfo->addonInfo->getSize() : 0;
    mPlayer->init();
}

AudioResourcePlayerKeeper::AudioResourcePlayerKeeper() {
    mResources = new sead::PtrArray<AudioResourcePlayer>();
    mResources->allocBuffer(2, nullptr);
}

void AudioResourcePlayerKeeper::createAndAppendAudioResourcePlayer(
    const char* name, const char* fileName, AudioSoundArchiveInfo* archiveInfo,
    IAudioResourceInfoAccessor* accessor) {
    auto* resources = mResources;
    for (s32 i = 0; i < resources->size(); i++)
        if (isEqualString(name, resources->unsafeAt(i)->getName()))
            return;

    mResources->pushBack(new AudioResourcePlayer(name, fileName, archiveInfo, accessor));
}

static inline const char* getMainArchiveName(AudioResourcePlayer* resourcePlayer) {
    if (isEqualString("Se", resourcePlayer->getName()))
        return "SeData";
    if (isEqualString("Bgm", resourcePlayer->getName()))
        return "BgmData";
    return "Main";
}

void AudioResourcePlayerKeeper::init(s32 size, bool a, bool b) {
    if (size > 0 && (size % 0x4000) == 0 && size >= 0x4000)
        mSize = size;

    for (s32 i = 0; i < mResources->size(); i++) {
        AudioResourcePlayer* resourcePlayer = mResources->unsafeAt(i);
        SeadAudioPlayer* player = resourcePlayer->getPlayer();
        player->getAudioSoundDataMgrMulti()->setContentRootPath("content:");
        player->getAudioSoundDataMgrMulti()->mountSoundArchiveFromFs(resourcePlayer->getFileName(),
                                                                     getCurrentHeap(), a, !b);

        if (_10 == 0) {
            alsd::AudioPlayerMulti::DataManagementSetupParam param;
            param._0 = _14;
            param.size = mSize;
            param._8 = 0;
            param.heap = getCurrentHeap();
            param.addonSize = resourcePlayer->getAddonSize();
            player->setupDataManagement(param);
        } else {
            player->setupDataManagement(_10, mSize, 0, getCurrentHeap(),
                                        resourcePlayer->getAddonSize());
        }

        player->addMainSoundArchiveToHolder(getMainArchiveName(resourcePlayer));
    }

    mIsInitialized = true;
}

AudioResourcePlayer* AudioResourcePlayerKeeper::tryGetAudioResourcePlayer(const char* name) const {
    for (s32 i = 0; i < mResources->size(); i++) {
        AudioResourcePlayer* resourcePlayer = mResources->unsafeAt(i);
        if (isEqualString(name, resourcePlayer->getName()))
            return resourcePlayer;
    }

    return nullptr;
}

SeadAudioPlayer* AudioResourcePlayerKeeper::tryGetSeadAudioPlayer(const char* name) const {
    AudioResourcePlayer* resourcePlayer = tryGetAudioResourcePlayer(name);

    if (resourcePlayer)
        return resourcePlayer->getPlayer();

    return nullptr;
}

void AudioResourcePlayerKeeper::update() {
    for (s32 i = 0; i < mResources->size(); i++)
        mResources->at(i)->getPlayer()->initialize();
}

}  // namespace al
