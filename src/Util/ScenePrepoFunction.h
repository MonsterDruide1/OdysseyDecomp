#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

class GameDataHolderAccessor;
class GameConfigData;

namespace rs {
bool trySavePrepoCompleteMainScenario(s32, s32, s64, s64, s64);
bool trySavePrepoStartStage(const char*, const sead::Vector3f&, s64, s64, s64);
bool trySavePrepoExitStage(const char*, const sead::Vector3f&, s64, s64, s64);
bool trySavePrepoMissEvent(const char*, const sead::Vector3f&, s64, s64, s64);
bool trySavePrepoShineGetEvent(const char*, s32, s32, s32, s64, s64, s64);
bool trySavePrepoReceiveAchievement(const char* s32, s64, s64, s64);
bool trySavePrepoAchievementProgress(GameDataHolderAccessor, s64, s64, s64);
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
s32 calcPrepoHashCode(const char*);
}  // namespace prepo
}  // namespace rs
