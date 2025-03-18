#pragma once

#include <basis/seadTypes.h>

namespace aal {
class IAudioFrameProcess;
}

namespace al {
class AudioDirector;
struct GameSystemInfo;
template <typename T>
class AudioInfoListWithParts;
class AudioResourceLoadGroupInfo;
class SeadAudioPlayer;
class PadRumbleDirector;
class AudioSystem;
class BgmDataBase;
class IUseSeadAudioPlayer;
}  // namespace al

namespace alAudioSystemFunction {
void getAudioSystemInfo(const al::GameSystemInfo*);
void getSeDataBase(const al::AudioDirector*);
void getBgmDataBase(const al::AudioDirector*);
void tryGetAudioStationedResource(const char*, const char*);
void tryGetAudioDebugStationedResource(const char*, const char*);
void addAudiioFrameProccess(al::AudioDirector*, aal::IAudioFrameProcess*);
void removeAudiioFrameProccess(al::AudioDirector*, aal::IAudioFrameProcess*);
void tryLoadAddonSoundArchive(const char*, al::SeadAudioPlayer*);
void loadAudioResource(const char*, al::AudioInfoListWithParts<al::AudioResourceLoadGroupInfo>*,
                       al::SeadAudioPlayer*, al::SeadAudioPlayer*);
void destroyAudioResource(const char*, al::AudioInfoListWithParts<al::AudioResourceLoadGroupInfo>*,
                          al::SeadAudioPlayer*, al::SeadAudioPlayer*);
void tryFindAudioPlayerRegistedSoundMemoryPoolHandler(const char*, al::SeadAudioPlayer*,
                                                      al::SeadAudioPlayer*);
void tryDisableSoundMemoryPoolHandler(const char*, al::SeadAudioPlayer*);
void tryDisableSoundMemoryPoolHandlerByFilePath(const char*, al::SeadAudioPlayer*);
void setPadRumbleDirectorForSe(al::AudioDirector*, al::PadRumbleDirector*);
void getSeadAudioPlayerForSe(const al::AudioSystem*);
void getSeadAudioPlayerForBgm(const al::AudioSystem*);
void tryCreateBgmMusicalInfoList(const char*, al::BgmDataBase*);
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
void getSeSoundHeapUsedSize(const al::AudioDirector*);
void getBgmSoundHeapUsedSize(const al::AudioDirector*);
void getHeapFreeSize(const al::AudioDirector*);
void getHeapSize(const al::AudioDirector*);
void enableAudioMaximizer(const al::GameSystemInfo*);
void disableAudioMaximizer(const al::GameSystemInfo*);
void loadSoundItem(al::IUseSeadAudioPlayer*, u32, u32);
void isLoadedSoundItem(al::IUseSeadAudioPlayer*, u32);
void saveHeapState(al::IUseSeadAudioPlayer*);
void loadHeapState(al::IUseSeadAudioPlayer*, s32);
void getCurrentHeapStateLevel(al::IUseSeadAudioPlayer*);
void getSoundResourceHeapFreeSize(al::IUseSeadAudioPlayer*);
void resetDataDependedStage(const al::AudioDirector*, const char*, s32);
}  // namespace alAudioSystemFunction
