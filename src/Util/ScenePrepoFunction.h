#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "System/GameDataHolderAccessor.h"

class GameConfigData;

namespace rs {
bool trySavePrepoCompleteMainScenario(s32 worldId, s32 mainScenarioNo, s64 playTime, s64 saveDataId,
                                      s64 acrossPlayTime);
bool trySavePrepoStartStage(const char* stageName, const sead::Vector3f& position, s64 playTime,
                            s64 saveDataId, s64 acrossPlayTime);
bool trySavePrepoExitStage(const char* stageName, const sead::Vector3f& position, s64 playTime,
                           s64 saveDataId, s64 acrossPlayTime);
bool trySavePrepoMissEvent(const char* stageName, const sead::Vector3f& position, s64 playTime,
                           s64 saveDataId, s64 acrossPlayTime);
bool trySavePrepoShineGetEvent(const char* stageName, s32 shineId, s32 totalShineNum,
                               s32 totalShopShineNum, s64 playTime, s64 saveDataId,
                               s64 acrossPlayTime);
bool trySavePrepoReceiveAchievement(const char* achievementName, s32 receivedNum, s64 playTime,
                                    s64 saveDataId, s64 acrossPlayTime);
bool trySavePrepoAchievementProgress(GameDataHolderAccessor accessor, s64 playTime, s64 saveDataId,
                                     s64 acrossPlayTime);
bool trySavePrepoGetCapEvent(const char*, s32, s64, s64, s64);
bool trySavePrepoGetClothEvent(const char*, s32, s64, s64, s64);
bool trySavePrepoGetGiftEvent(const char*, s32, s64, s64, s64);
bool trySavePrepoGetStickerEvent(const char*, s32, s64, s64, s64);
bool trySavePrepoChangeCapEvent(const char*, s64, s64, s64);
bool trySavePrepoChangeClothEvent(const char*, s64, s64, s64);
bool trySavePrepoFirstHackEvent(const char*, s32, s64, s64, s64);
bool trySavePrepoGetCollectBgmEvent(const char*, const char*, bool, s32, s64, s64, s64);
bool trySavePrepoSettingsState(bool, const char*, const GameConfigData&, s64, s64);
bool trySavePrepoSeparatePlayMode(bool, s64, s64, s64);

namespace prepo {
void enableIsSavePrepo();
void disableIsSavePrepo();
s64 generateSaveDataId();
s32 calcPrepoHashCode(const char* str);
}  // namespace prepo
}  // namespace rs
