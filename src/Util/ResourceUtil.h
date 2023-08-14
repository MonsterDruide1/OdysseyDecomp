#pragma once

#include "System/GameDataHolderAccessor.h"

namespace rs {

bool checkGetAchievement(GameDataHolderAccessor, const char*);

s32 calcClearWorldNum(GameDataHolderAccessor);
s32 calcBuyItemNumForCoinCollectByWorld(GameDataHolderAccessor);
s32 calcGetShineNumDot(GameDataHolderAccessor);
s32 calcGetShineNumTreasureBox(GameDataHolderAccessor);
s32 calcGetShineNumNoteObj(GameDataHolderAccessor);
s32 calcGetShineNumTimerAthletic(GameDataHolderAccessor);
s32 calcGetShineNumKinopioBrigade(GameDataHolderAccessor);
s32 calcGetShineNumWorldTravelingPeach(GameDataHolderAccessor);
s32 calcGetShineNumCollectAnimal(GameDataHolderAccessor);
s32 calcGetShineNumKuriboGirl(GameDataHolderAccessor);
s32 calcGetShineNumJugemFish(GameDataHolderAccessor);
s32 calcGetShineNumGrowPlant(GameDataHolderAccessor);
s32 calcGetShineNumRabbit(GameDataHolderAccessor);
s32 calcGetShineNumDigPoint(GameDataHolderAccessor);
s32 calcGetShineNumCapHanger(GameDataHolderAccessor);
s32 calcGetShineNumBird(GameDataHolderAccessor);
s32 calcGetShineNumCostumeRoom(GameDataHolderAccessor);
s32 calcGetShineNumSlot(GameDataHolderAccessor);
s32 calcGetShineNumHideAndSeekCapMan(GameDataHolderAccessor);
s32 calcGetShineNumCollectedBgm(GameDataHolderAccessor);
s32 calcGetShineNumHintPhoto(GameDataHolderAccessor);
s32 calcGetShineNumRace(GameDataHolderAccessor);
s32 calcGetShineNumFigureWalking(GameDataHolderAccessor);
s32 calcSphinxQuizCompleteNum(GameDataHolderAccessor);
s32 calcHaveStickerNum(GameDataHolderAccessor);
s32 calcHaveGiftNum(GameDataHolderAccessor);
s32 calcHackObjNum(GameDataHolderAccessor);
s32 calcHaveCapNum(GameDataHolderAccessor);
s32 calcHaveClothNum(GameDataHolderAccessor);
s32 calcUnlockMoonRockNum(GameDataHolderAccessor);
s32 calcWorldWarpHoleThroughNum(GameDataHolderAccessor);
s32 calcGetCheckpointNum(GameDataHolderAccessor);
s32 getTotalCoinNum(GameDataHolderAccessor);
s32 getPlayerJumpCount(GameDataHolderAccessor);
s32 getPlayerThrowCapCount(GameDataHolderAccessor);

}  // namespace rs
