#pragma once

#include "System/GameDataHolderAccessor.h"

namespace rs {

bool checkGetAchievement(GameDataHolderAccessor, const char*);

int calcClearWorldNum(GameDataHolderAccessor);
int calcBuyItemNumForCoinCollectByWorld(GameDataHolderAccessor);
int calcGetShineNumDot(GameDataHolderAccessor);
int calcGetShineNumTreasureBox(GameDataHolderAccessor);
int calcGetShineNumNoteObj(GameDataHolderAccessor);
int calcGetShineNumTimerAthletic(GameDataHolderAccessor);
int calcGetShineNumKinopioBrigade(GameDataHolderAccessor);
int calcGetShineNumWorldTravelingPeach(GameDataHolderAccessor);
int calcGetShineNumCollectAnimal(GameDataHolderAccessor);
int calcGetShineNumKuriboGirl(GameDataHolderAccessor);
int calcGetShineNumJugemFish(GameDataHolderAccessor);
int calcGetShineNumGrowPlant(GameDataHolderAccessor);
int calcGetShineNumRabbit(GameDataHolderAccessor);
int calcGetShineNumDigPoint(GameDataHolderAccessor);
int calcGetShineNumCapHanger(GameDataHolderAccessor);
int calcGetShineNumBird(GameDataHolderAccessor);
int calcGetShineNumCostumeRoom(GameDataHolderAccessor);
int calcGetShineNumSlot(GameDataHolderAccessor);
int calcGetShineNumHideAndSeekCapMan(GameDataHolderAccessor);
int calcGetShineNumCollectedBgm(GameDataHolderAccessor);
int calcGetShineNumHintPhoto(GameDataHolderAccessor);
int calcGetShineNumRace(GameDataHolderAccessor);
int calcGetShineNumFigureWalking(GameDataHolderAccessor);
int calcSphinxQuizCompleteNum(GameDataHolderAccessor);
int calcHaveStickerNum(GameDataHolderAccessor);
int calcHaveGiftNum(GameDataHolderAccessor);
int calcHackObjNum(GameDataHolderAccessor);
int calcHaveCapNum(GameDataHolderAccessor);
int calcHaveClothNum(GameDataHolderAccessor);
int calcUnlockMoonRockNum(GameDataHolderAccessor);
int calcWorldWarpHoleThroughNum(GameDataHolderAccessor);
int calcGetCheckpointNum(GameDataHolderAccessor);
int getTotalCoinNum(GameDataHolderAccessor);
int getPlayerJumpCount(GameDataHolderAccessor);
int getPlayerThrowCapCount(GameDataHolderAccessor);

}  // namespace rs
