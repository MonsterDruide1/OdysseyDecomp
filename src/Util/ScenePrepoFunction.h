#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "System/GameDataHolderAccessor.h"

class GameConfigData;

namespace rs {
enum InOutType : s64 { InOutType_Start, InOutType_Exit, InOutType_Miss };

enum ItemCategoryType : s64 {
    ItemCategoryType_Cap,
    ItemCategoryType_Cloth,
    ItemCategoryType_Gift,
    ItemCategoryType_Sticker
};

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
bool trySavePrepoGetCapEvent(const char* itemName, s32 totalNum, s64 playTime, s64 saveDataId,
                             s64 acrossPlayTime);
bool trySavePrepoGetClothEvent(const char* itemName, s32 totalNum, s64 playTime, s64 saveDataId,
                               s64 acrossPlayTime);
bool trySavePrepoGetGiftEvent(const char* itemName, s32 totalNum, s64 playTime, s64 saveDataId,
                              s64 acrossPlayTime);
bool trySavePrepoGetStickerEvent(const char* itemName, s32 totalNum, s64 playTime, s64 saveDataId,
                                 s64 acrossPlayTime);
bool trySavePrepoChangeCapEvent(const char* itemName, s64 playTime, s64 saveDataId,
                                s64 acrossPlayTime);
bool trySavePrepoChangeClothEvent(const char* itemName, s64 playTime, s64 saveDataId,
                                  s64 acrossPlayTime);
bool trySavePrepoFirstHackEvent(const char* hackObjName, s32 totalHackedObjNum, s64 playTime,
                                s64 saveDataId, s64 acrossPlayTime);
bool trySavePrepoGetCollectBgmEvent(const char* bgmResourceName, const char* bgmSituationName,
                                    bool isBailout, s32 collectedBgmNum, s64 playTime,
                                    s64 saveDataId, s64 acrossPlayTime);
bool trySavePrepoSettingsState(bool isKidsMode, const char* language,
                               const GameConfigData& configData, s64 saveDataId,
                               s64 acrossPlayTime);
bool trySavePrepoSeparatePlayMode(bool isSeparatePlayMode, s64 playTime, s64 saveDataId,
                                  s64 acrossPlayTime);

namespace prepo {
void enableIsSavePrepo();
void disableIsSavePrepo();
s64 generateSaveDataId();
u32 calcPrepoHashCode(const char* str);
}  // namespace prepo
}  // namespace rs
