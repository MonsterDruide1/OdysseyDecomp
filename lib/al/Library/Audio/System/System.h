#pragma once

#include <basis/seadTypes.h>
#include <hostio/seadHostIONode.h>
#include <nn/atk/SoundArchivePlayer.h>
#include <nn/atk/SoundDataManager.h>

#include "Project/AlsdAudioPlayer.h"

namespace al {
class ByamlIter;
class AudioInfoListCreateFunctorBase;
}  // namespace al

namespace nn::atk {
class SoundArchive;

namespace SoundStartable {
class StartInfo;
}
}  // namespace nn::atk

namespace alsd {
class SoundHandle;
class AudioSoundDataMgrMulti;
class AudioSoundHeapMulti;
class AudioFsSoundArchiveCafe;

class AudioPlayerMulti : public AudioPlayer,
                         public nn::atk::SoundArchivePlayer,
                         public sead::hostio::Node {
    SEAD_RTTI_OVERRIDE(AudioPlayerMulti, AudioPlayer)
public:
    struct DataManagementSetupParam {
        f32 _0;
        s32 size;
        s32 _8;
        sead::Heap* heap = nullptr;
        s32 addonSize = 0;
    };

    static_assert(sizeof(DataManagementSetupParam) == 0x20);

    AudioPlayerMulti();

    void initialize() override;
    void finalize() override;
    void calc() override;
    bool startSound(SoundHandle*, u32) override;
    bool startSound(SoundHandle*, const char*) override;
    bool holdSound(SoundHandle*, u32) override;
    bool holdSound(SoundHandle*, const char*) override;
    s32 getSoundCount() const override;
    const char* getSoundName(u32) const override;
    s32 getSoundId(const char*) const override;

    void stopAll(s32);
    void destroySoundHeap();
    void shutdownDataManagement();
    void pauseAll(s32);
    void unpauseAll(s32);
    s32 detail_SetupSound(nn::atk::SoundHandle*, u32, bool, const char*,
                          const nn::atk::SoundStartable::StartInfo*);
    void createSoundHeap(u64, sead::Heap*);
    void setupDataManagement(u32, u32, u32, sead::Heap*, s32);
    void setupDataManagement(const DataManagementSetupParam&);
    bool isPlayerBufferCrossingMemoryBoundary() const;
    bool isStreamBufferCrossingMemoryBoundary() const;
    void genMessage(sead::hostio::Context*);
    void listenPropertyEvent(const sead::hostio::PropertyEvent*);

    AudioSoundDataMgrMulti* getAudioSoundDataMgrMulti() const { return mAudioSoundDataMgrMulti; }

private:
    bool isNwEnabled_() const;
    void setPauseAll_(s32, bool);
    bool setupDataManagementInner_(const nn::atk::SoundArchive&, u32, u32, u32, sead::Heap*, s32);

    char filler_2f8[0x38];
    AudioSoundDataMgrMulti* mAudioSoundDataMgrMulti;
    char filler_338[0x58];
};

static_assert(sizeof(AudioPlayerMulti) == 0x390);

class AudioSoundDataMgrMulti : public nn::atk::SoundDataManager {
public:
    AudioSoundDataMgrMulti();

    void connectSoundHeap(AudioSoundHeapMulti*);
    AudioFsSoundArchiveCafe* getSoundArchive() const;
    void setContentRootPath(const char*);
    bool mountSoundArchiveFromFs(const sead::SafeString&, sead::Heap*, bool, bool);
    bool mountSoundArchiveFromMemory(const void*, sead::Heap*);
    void unmountSoundArchive();
    bool loadData(u32, u32, u32, AudioSoundHeapMulti*);
    bool loadData(const char*, u32, u32, AudioSoundHeapMulti*);

private:
    bool isNwEnabled_() const;
    bool setupManager_(sead::Heap*);
    bool tryGetDefaultSoundHeapAndCheckReady_(AudioSoundHeapMulti**) const;

    char filler[0x20];
};

static_assert(sizeof(AudioSoundDataMgrMulti) == 0x268);

}  // namespace alsd

namespace alAudioInfoListFunction {
s32 getCreateAudioInfoListSize(const al::ByamlIter&, s32);
s32 getCreateAudioInfoListSize(const al::ByamlIter&, const al::ByamlIter&);
void createAudioInfoAndSetToList(al::AudioInfoListCreateFunctorBase*, const al::ByamlIter&);
}  // namespace alAudioInfoListFunction
