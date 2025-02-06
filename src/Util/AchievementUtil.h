#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>
#include <time/seadDateTime.h>

#include "System/GameDataHolderAccessor.h"

namespace al {
class IUseMessageSystem;
class LayoutActor;
class LiveActor;
}  // namespace al

class AchievementHolder;
class AchievementNpc;

namespace rs {

const char16* getAchievementName(const al::IUseMessageSystem* user, const char* label);
const char16* getAchievementName(const al::LayoutActor* layout, s32 achievementIdx);
const char16* getAchievementHint(const al::IUseMessageSystem* user, const char* label);
const char16* getAchievementHint(const al::LayoutActor* layout, s32 achievementIdx);
AchievementHolder* getAchievementHolder(const al::LayoutActor* layout);
void unlockAchievementShineName(GameDataHolderAccessor accessor);
bool isUnlockAchievementShineName(GameDataHolderAccessor accessor);
bool checkGetAchievement(GameDataHolderAccessor accessor, const char* label);
bool checkGetAchievement(const al::LayoutActor* layout, s32 achievementIdx);
sead::DateTime findAchievementGetTime(const al::LayoutActor* layout, s32 achievementIdx);
sead::DateTime findAchievementGetTime(const al::LayoutActor* layout, const char* label);
sead::Vector3f* findAchievementTrans(const al::LayoutActor* layout, s32 achievementIdx);
s32 getAchievementNum(const al::LayoutActor* layout);
s32 calcReceivedAchievementNum(const GameDataHolder* holder);
s32 getTotalCoinNum(const GameDataHolder* holder);
s32 calcWorldWarpHoleThroughNum(const GameDataHolder* holder);
s32 getWorldWarpHoleThroughNumMax(const GameDataHolder* holder);
s32 calcGetCheckpointNum(const GameDataHolder* holder);
s32 calcBuyItemNumForCoinCollectByWorld(const GameDataHolder* holder);
s32 calcClearWorldNum(const GameDataHolder* holder);
void addPlayerJumpCount(const al::LiveActor* actor);
s32 getPlayerJumpCount(const GameDataHolder* holder);
void addPlayerThrowCapCount(const al::LiveActor* actor);
s32 getPlayerThrowCapCount(const GameDataHolder* holder);
bool checkGetCoinCollectAllInWorld(const al::LiveActor* actor, s32 worldId);
bool checkGetCoinCollectHalfInWorld(const AchievementNpc* achievementNpc, s32 worldId);
bool checkGetShineAllInWorld(const AchievementNpc* achievementNpc, s32 worldId);
bool checkGetClothAndCapNum(const AchievementNpc* achievementNpc, s32 num);
bool checkUnlockMoonRockAllWorld(GameDataHolderAccessor accessor);
s32 calcUnlockMoonRockNum(GameDataHolderAccessor accessor);
s32 calcHackObjNum(GameDataHolderAccessor accessor);
s32 calcSphinxQuizCompleteNum(GameDataHolderAccessor accessor);
s32 calcGetShineNumDot(const GameDataHolder* holder);
s32 calcGetShineNumTreasureBox(const GameDataHolder* holder);
s32 calcGetShineNumNoteObj(const GameDataHolder* holder);
s32 calcGetShineNumTimerAthletic(const GameDataHolder* holder);
s32 calcGetShineNumKinopioBrigade(const GameDataHolder* holder);
s32 calcGetShineNumWorldTravelingPeach(const GameDataHolder* holder);
s32 calcGetShineNumCollectAnimal(const GameDataHolder* holder);
s32 calcAllShineNumCollectAnimal(const GameDataHolder* holder);
s32 calcGetShineNumKuriboGirl(const GameDataHolder* holder);
s32 calcGetShineNumJugemFish(const GameDataHolder* holder);
s32 calcGetShineNumGrowPlant(const GameDataHolder* holder);
s32 calcGetShineNumRabbit(const GameDataHolder* holder);
s32 calcGetShineNumDigPoint(const GameDataHolder* holder);
s32 calcGetShineNumCapHanger(const GameDataHolder* holder);
s32 calcGetShineNumBird(const GameDataHolder* holder);
s32 calcGetShineNumCostumeRoom(const GameDataHolder* holder);
s32 calcGetShineNumSlot(const GameDataHolder* holder);
s32 calcGetShineNumRace(const GameDataHolder* holder);
s32 calcGetShineNumFigureWalking(const GameDataHolder* holder);
s32 calcGetShineNumHideAndSeekCapMan(const GameDataHolder* holder);
s32 calcAllShineNumHideAndSeekCapMan(const GameDataHolder* holder);
s32 calcGetShineNumCollectBgmNpc(const GameDataHolder* holder);
s32 calcAllShineNumCollectBgmNpc(const GameDataHolder* holder);
s32 calcGetShineNumHintPhoto(const GameDataHolder* holder);

}  // namespace rs
