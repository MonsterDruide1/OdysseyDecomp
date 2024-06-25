#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

namespace al {
class IUseAudioKeeper;
class ISeModifier;
class MeInfo;
class SePlayParamList;

void startSe(const al::IUseAudioKeeper*, const sead::SafeString&);
bool verifySeKeeperNull(const al::IUseAudioKeeper*, const sead::SafeString&);
bool tryStartSe(const al::IUseAudioKeeper*, const sead::SafeString&);
void holdSe(const al::IUseAudioKeeper*, const sead::SafeString&);
bool tryHoldSe(const al::IUseAudioKeeper*, const sead::SafeString&);
void startSeWithParam(const al::IUseAudioKeeper*, const sead::SafeString&, f32, const char*);
bool tryStartSeWithParam(const al::IUseAudioKeeper*, const sead::SafeString&, f32, const char*);
void holdSeWithParam(const al::IUseAudioKeeper*, const sead::SafeString&, f32, const char*);
bool tryHoldSeWithParam(const al::IUseAudioKeeper*, const sead::SafeString&, f32, const char*);
void startSeWithMeInfo(const al::IUseAudioKeeper*, const sead::SafeString&, al::MeInfo const*);
void holdSeWithMeInfo(const al::IUseAudioKeeper*, const sead::SafeString&, al::MeInfo const*);
void startSeWithManualControl(const al::IUseAudioKeeper*, const sead::SafeString&,
                              const al::SePlayParamList*, const char*, const sead::Vector3f*);
bool tryStartSeWithManualControl(const al::IUseAudioKeeper*, const sead::SafeString&,
                                 const al::SePlayParamList*, const char*, const sead::Vector3f*);
void holdSeWithManualControl(const al::IUseAudioKeeper*, const sead::SafeString&,
                             const al::SePlayParamList*, const char*, const sead::Vector3f*);
bool tryHoldSeWithManualControl(const al::IUseAudioKeeper*, const sead::SafeString&,
                                const al::SePlayParamList*, const char*, const sead::Vector3f*);
void stopSe(const al::IUseAudioKeeper*, const sead::SafeString&, s32, const char*);
void stopSeWithoutFreezed(const al::IUseAudioKeeper*, const sead::SafeString&, s32, const char*);
bool tryStopSe(const al::IUseAudioKeeper*, const sead::SafeString&, s32, const char*);
bool tryStopAllSeFromUser(const al::IUseAudioKeeper*, s32, const char*);
void stopAllSeFromUser(const al::IUseAudioKeeper*, s32, const char*);
bool isPlayingAnySe(const al::IUseAudioKeeper*, const char*);
void checkIsPlayingSe(const al::IUseAudioKeeper*, const sead::SafeString&, const char*);
void setLifeTimeForHoldCall(const al::IUseAudioKeeper*, const char*, s32, const char*);
void setSeSourceVolume(const al::IUseAudioKeeper*, f32);
void setSeKeeperPlayNamePrefix(const al::IUseAudioKeeper*, const char*);
void resetSeKeeperPlayNamePrefix(const al::IUseAudioKeeper*);
bool isExistSeKeeper(const al::IUseAudioKeeper*);
void forceActivateSeKeeper(al::IUseAudioKeeper*);
void validateSeKeeper(al::IUseAudioKeeper*);
void invalidateSeKeeper(al::IUseAudioKeeper*);
void setSeEmitterPoseMtxPtr(const al::IUseAudioKeeper*, const sead::Matrix34f*, const char*);
void setSeEmitterPosePosPtr(const al::IUseAudioKeeper*, const sead::Vector3f*, const char*);
bool tryUpdateSeMaterialCode(al::IUseAudioKeeper*, const char*);
void updateSeMaterialWater(al::IUseAudioKeeper*, bool);
void updateSeMaterialWet(al::IUseAudioKeeper*, bool);
void updateSeMaterialBeyondWall(al::IUseAudioKeeper*, bool);
void updateSeMaterialPuddle(al::IUseAudioKeeper*, bool);
void resetSeMaterialName(const al::IUseAudioKeeper*);
void setSeModifier(const al::IUseAudioKeeper*, al::ISeModifier*);
void setSeUserSyncParamPtr(const al::IUseAudioKeeper*, const f32*, const char*);
void setSeUserSyncParamPtrInt(const al::IUseAudioKeeper*, const s32*, const char*);
void setSeOutputFromController(al::SePlayParamList*, s32, bool);
void setSeOutputTvDrcRemoteAll(al::SePlayParamList*);
}  // namespace al
