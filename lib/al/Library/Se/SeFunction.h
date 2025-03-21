#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

namespace al {
class IUseAudioKeeper;
class ISeModifier;
class MeInfo;
class SePlayParamList;
class AudioDirector;

void startSe(const IUseAudioKeeper*, const sead::SafeString&);
bool verifySeKeeperNull(const IUseAudioKeeper*, const sead::SafeString&);
bool tryStartSe(const IUseAudioKeeper*, const sead::SafeString&);
void holdSe(const IUseAudioKeeper*, const sead::SafeString&);
bool tryHoldSe(const IUseAudioKeeper*, const sead::SafeString&);
void startSeWithParam(const IUseAudioKeeper*, const sead::SafeString&, f32, const char*);
bool tryStartSeWithParam(const IUseAudioKeeper*, const sead::SafeString&, f32, const char*);
void holdSeWithParam(const IUseAudioKeeper*, const sead::SafeString&, f32, const char*);
bool tryHoldSeWithParam(const IUseAudioKeeper*, const sead::SafeString&, f32, const char*);
void startSeWithMeInfo(const IUseAudioKeeper*, const sead::SafeString&, const MeInfo*);
void holdSeWithMeInfo(const IUseAudioKeeper*, const sead::SafeString&, const MeInfo*);
void startSeWithManualControl(const IUseAudioKeeper*, const sead::SafeString&,
                              const SePlayParamList*, const char*, const sead::Vector3f*);
bool tryStartSeWithManualControl(const IUseAudioKeeper*, const sead::SafeString&,
                                 const SePlayParamList*, const char*, const sead::Vector3f*);
void holdSeWithManualControl(const IUseAudioKeeper*, const sead::SafeString&,
                             const SePlayParamList*, const char*, const sead::Vector3f*);
bool tryHoldSeWithManualControl(const IUseAudioKeeper*, const sead::SafeString&,
                                const SePlayParamList*, const char*, const sead::Vector3f*);
void stopSe(const IUseAudioKeeper*, const sead::SafeString&, s32, const char*);
void stopSeWithoutFreezed(const IUseAudioKeeper*, const sead::SafeString&, s32, const char*);
bool tryStopSe(const IUseAudioKeeper*, const sead::SafeString&, s32, const char*);
void tryStopAllSeFromUser(const IUseAudioKeeper*, s32, const char*);
void stopAllSeFromUser(const IUseAudioKeeper*, s32, const char*);
bool isPlayingAnySe(const IUseAudioKeeper*, const char*);
bool checkIsPlayingSe(const IUseAudioKeeper*, const sead::SafeString&, const char*);
void setLifeTimeForHoldCall(const IUseAudioKeeper*, const char*, s32, const char*);
void setSeSourceVolume(const IUseAudioKeeper*, f32);
void setSeKeeperPlayNamePrefix(const IUseAudioKeeper*, const char*);
void resetSeKeeperPlayNamePrefix(const IUseAudioKeeper*);
bool isExistSeKeeper(const IUseAudioKeeper*);
void forceActivateSeKeeper(IUseAudioKeeper*);
void validateSeKeeper(IUseAudioKeeper*);
void invalidateSeKeeper(IUseAudioKeeper*);
void setSeEmitterPoseMtxPtr(const IUseAudioKeeper*, const sead::Matrix34f*, const char*);
void setSeEmitterPosePosPtr(const IUseAudioKeeper*, const sead::Vector3f*, const char*);
void tryUpdateSeMaterialCode(IUseAudioKeeper*, const char*);

void updateSeMaterialWater(IUseAudioKeeper*, bool);
void updateSeMaterialWet(IUseAudioKeeper*, bool);
void updateSeMaterialBeyondWall(IUseAudioKeeper*, bool);
void updateSeMaterialPuddle(IUseAudioKeeper*, bool);
void resetSeMaterialName(const IUseAudioKeeper*);
void setSeModifier(const IUseAudioKeeper*, ISeModifier*);
void setSeUserSyncParamPtr(const IUseAudioKeeper*, const f32*, const char*);
void setSeUserSyncParamPtrInt(const IUseAudioKeeper*, const s32*, const char*);
void setSeOutputFromController(SePlayParamList*, s32, bool);
void setSeOutputTvDrcRemoteAll(SePlayParamList*);
}  // namespace al

namespace alSeFunction {
void stopAllSe(const al::AudioDirector*, u32);
}
