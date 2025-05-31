#pragma once

#include <basis/seadTypes.h>

namespace aal {
class IAudioFrameProcess;
}

namespace al {
class AudioDirector;
class AudioEffectDataBase;
template <typename T>
class AudioInfoListWithParts;
class AudioResourceLoadGroupInfo;
class AudioResourceLoadingInfo;
class AudioResourcePlayerKeeper;
class AudioSystem;
class AudioSystemDebug;
struct AudioSystemInfo;
class AudioSystemInitInfo;
class BgmDataBase;
class BgmMusicalInfo;
struct GameSystemInfo;
class IUseSeadAudioPlayer;
class PadRumbleDirector;
class Resource;
class SeadAudioPlayer;
class SeDataBase;
}  // namespace al

namespace alAudioSystemFunction {
al::AudioSystemInfo* getAudioSystemInfo(const al::GameSystemInfo*);
al::SeDataBase* getSeDataBase(const al::AudioDirector*);
al::BgmDataBase* getBgmDataBase(const al::AudioDirector*);
al::Resource* tryGetAudioStationedResource(const char*, const char*);
al::Resource* tryGetAudioDebugStationedResource(const char*, const char*);
bool addAudiioFrameProccess(al::AudioDirector*, aal::IAudioFrameProcess*);
void removeAudiioFrameProccess(al::AudioDirector*, aal::IAudioFrameProcess*);
void tryLoadAddonSoundArchive(const char*, al::SeadAudioPlayer*);
void loadAudioResource(const char*, al::AudioInfoListWithParts<al::AudioResourceLoadGroupInfo>*,
                       al::SeadAudioPlayer*, al::SeadAudioPlayer*);
void destroyAudioResource(const char*, al::AudioInfoListWithParts<al::AudioResourceLoadGroupInfo>*,
                          al::SeadAudioPlayer*, al::SeadAudioPlayer*);
al::SeadAudioPlayer* tryFindAudioPlayerRegistedSoundMemoryPoolHandler(const char*,
                                                                      al::SeadAudioPlayer*,
                                                                      al::SeadAudioPlayer*);
bool tryDisableSoundMemoryPoolHandler(const char*, al::SeadAudioPlayer*);
bool tryDisableSoundMemoryPoolHandlerByFilePath(const char*, al::SeadAudioPlayer*);
void setPadRumbleDirectorForSe(al::AudioDirector*, al::PadRumbleDirector*);
al::SeadAudioPlayer* getSeadAudioPlayerForSe(const al::AudioSystem*);
al::SeadAudioPlayer* getSeadAudioPlayerForBgm(const al::AudioSystem*);
al::BgmMusicalInfo* tryCreateBgmMusicalInfoList(const char*, al::BgmDataBase*);
void stopAllSound(const al::AudioDirector*, s32, bool);
void pauseAudioDirector(al::AudioDirector*, bool, u32);
void pauseAudioDirectorForDebug(al::AudioDirector*, bool, u32);
void pauseBgmDirector(al::AudioDirector*, bool, u32);
void pauseAllSeForDemo(const al::AudioDirector*, bool, u32);
void pauseAllSe(const al::AudioDirector*, bool, u32);
void startBgmDucking(const al::AudioDirector*);
void endBgmDucking(const al::AudioDirector*);
void clearBgmDataForStepOverScene(const al::AudioDirector*);
void softReset(const al::AudioDirector*, const al::AudioDirector*);
void notifyIsModeHandheld(const al::AudioDirector*, bool);
s32 getSeSoundHeapUsedSize(const al::AudioDirector*);
s32 getBgmSoundHeapUsedSize(const al::AudioDirector*);
u64 getHeapFreeSize(const al::AudioDirector*);
u64 getHeapSize(const al::AudioDirector*);
void enableAudioMaximizer(const al::GameSystemInfo*);
void disableAudioMaximizer(const al::GameSystemInfo*);
bool loadSoundItem(al::IUseSeadAudioPlayer*, u32, u32);
bool isLoadedSoundItem(al::IUseSeadAudioPlayer*, u32);
s32 saveHeapState(al::IUseSeadAudioPlayer*);
void loadHeapState(al::IUseSeadAudioPlayer*, s32);
s32 getCurrentHeapStateLevel(al::IUseSeadAudioPlayer*);
u64 getSoundResourceHeapFreeSize(al::IUseSeadAudioPlayer*);
void resetDataDependedStage(const al::AudioDirector*, const char*, s32);
}  // namespace alAudioSystemFunction

namespace al {
class AudioSystem {
public:
    AudioSystem();

    void init(const AudioSystemInitInfo&);
    void initDataBase();
    void initResourcePlayer(const AudioSystemInitInfo&);
    void applyDeviceVolume();
    void updateHWOutputSetting();
    void changeFinalMixInputBgmChVolume();
    void initDebugModule(const AudioSystemInitInfo&);
    void initSystemInfo();
    void update();
    void finalize();
    void addAudiioFrameProccess(aal::IAudioFrameProcess*);
    void removeAudiioFrameProccess(aal::IAudioFrameProcess*);
    void pauseSystemImmediately(bool, const char*, bool);

    AudioSystemInfo* getAudioSystemInfo() const { return mAudioSystemInfo; }

private:
    AudioResourcePlayerKeeper* mAudioResourcePlayerKeeper;
    AudioResourceLoadingInfo* mAudioResourceLoadingInfo;
    SeDataBase* mSeDataBase;
    BgmDataBase* mBgmDataBase;
    BgmMusicalInfo* mBgmMusicalInfo;
    AudioSystemInfo* mAudioSystemInfo;
    AudioSystemDebug* mAudioSystemDebug;
    f32 mMasterVolume;
    f32 mTvOutputVolume;
    f32 mConsoleVolume;
    AudioEffectDataBase* mAudioEffectDataBase;
    char filler[0x90];
};

static_assert(sizeof(AudioSystem) == 0xe0);
}  // namespace al
