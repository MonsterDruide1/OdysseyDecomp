#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <nn/atk.h>
#include <prim/seadSafeString.hpp>

#include "Library/Audio/System/System.h"

namespace sead {
class Heap;
}  // namespace sead

namespace alsd {
class AudioMemorySoundArchiveCafe;
}  // namespace alsd

namespace al {
struct AudioSoundArchiveInfo;
class IAudioResourceInfoAccessor;
class SoundMemoryPoolHandler;
class AcLSoundHandle;

class SeadAudioPlayer : public alsd::AudioPlayerMulti {
public:
    SeadAudioPlayer(AudioSoundArchiveInfo*, IAudioResourceInfoAccessor*);

    virtual void loadSoundItem(const sead::SafeString&, u32);
    virtual void loadSoundItem(u32, u32);
    virtual u32 getItemType(u32) const;
    virtual nn::atk::SoundArchive* getSoundArchive(const char*) const;

    void init();
    bool isLoadedSoundItem(u32, u32) const;
    bool startAudio(AcLSoundHandle*, u32);
    // TODO: Replace return type with nn::atk enum
    u32 getSoundType(u32) const;
    alsd::AudioMemorySoundArchiveCafe* getSoundArchive() const;
    bool readSequenceSoundInfo(nn::atk::SoundArchive::SequenceSoundInfo*, u32) const;
    s32 getBankInfoSize(u32) const;
    s32 getBankWaveArcSize(u32) const;
    s32 getWaveArcSize(u32) const;
    s32 getSoundItemSize(u32) const;
    s32 getBankTotalSize(u32) const;
    s32 getSequenceSoundSize(u32) const;
    s32 getSoundPlayerCount() const;
    s32 getSoundPlayerIdFromIndex(s32);
    f32 getAudioResourceVolume(u32) const;
    void setSoundPlayerVolume(const char*, f32);
    f32 getSoundPlayerVolume(const char*);
    bool isWaveSound(const char*) const;
    bool isStreamSound(const char*) const;
    bool isSequenceSound(const char*) const;
    bool isStereoWaveSound(u32) const;
    sead::Heap* getSeadAudioSoundHeap() const;
    void trySetSoundMemoryPoolHandler(SoundMemoryPoolHandler*);
    SoundMemoryPoolHandler* tryGetSoundMemoryPoolHandler(const char*);
    SoundMemoryPoolHandler* tryGetSoundMemoryPoolHandlerByFilePath(const char*);
    bool removeSoundMemoryPoolHandler(SoundMemoryPoolHandler*);
    void addAddonSoundArchive(SoundMemoryPoolHandler*);
    void removeAddonSoundArchive(SoundMemoryPoolHandler*);
    bool addMainSoundArchiveToHolder(const char*);
    bool isLoadedArchive(const char*);
    bool isLoadedResource(const char*, const char*);

private:
    sead::PtrArray<void>* _390;
    sead::PtrArray<void>* _398;
    IAudioResourceInfoAccessor* mResourceInfoAccessor;
};

static_assert(sizeof(SeadAudioPlayer) == 0x3a8);
}  // namespace al
