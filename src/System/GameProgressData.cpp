#include "System/GameProgressData.h"

#include "Library/Yaml/ByamlUtil.h"
#include "Library/Yaml/Writer/ByamlWriter.h"

#include "System/GameDataFunction.h"
#include "System/WorldList.h"

GameProgressData::GameProgressData(const WorldList* worldList) : mWorldList(worldList) {
    s32 worldNum = worldList->getWorldNum();

    mIsUnlockWorld = new bool[worldNum];
    mWorldIdForWorldWarpHole = new s32[worldNum];
    mWorldIdForShineList = new s32[worldNum];
    mWorldIdForWorldMap = new s32[worldNum];
    mIsFirstTimeWorld = new bool[worldNum];

    init();
}

void GameProgressData::init() {
    mHomeStatus = 0;
    mHomeLevel = 0;
    mUnlockWorldStatusFirstBranch = 0;
    mUnlockWorldStatusSecondBranch = 0;
    mUnlockWorldNum = 1;
    mWaterfallWorldProgress = 0;

    for (s32 i = 0; i < mWorldList->getWorldNum(); i++)
        mIsFirstTimeWorld[i] = true;

    mIsFirstTimeWorld[0] = false;

    updateList();
}

void GameProgressData::updateList() {
    initList();

    s32 idxPeach = mWorldList->tryFindWorldIndexByDevelopName("Peach");

    for (s32 i = 0, j = 0; i < mWorldList->getWorldNum(); i++) {
        if (calcWorldIdByOrderUnlock(i) != -1) {
            j++;
            mIsUnlockWorld[i] = true;
            if (j == mUnlockWorldNum) break;
        }
    }

    s32 idxCloud = mWorldList->tryFindWorldIndexByDevelopName("Cloud");
    s32 idxAttack = mWorldList->tryFindWorldIndexByDevelopName("Attack");
    s32 idxCity = mWorldList->tryFindWorldIndexByDevelopName("City");
    s32 idxSky = mWorldList->tryFindWorldIndexByDevelopName("Sky");

    bool isUnlockPeach = mIsUnlockWorld[idxPeach];

    // if (!isUnlockPeach) {
    //     for (s32 i = 0; i < mWorldList->getWorldNum(); i++) {
    //         s32 worldId = calcWorldIdByOrderUnlock(i);
    //         if (worldId != -1) {
    //             if (worldId == idxCloud && mUnlockWorldNum == idxCloud + 1)
    //                 mWorldIdForWorldMap[idxCloud] = idxCity;
    //             else if (worldId == idxAttack && mUnlockWorldNum == idxAttack + 1)
    //                 mWorldIdForWorldMap[idxAttack] = idxSky;
    //             else
    //                 mWorldIdForWorldMap[i] = worldId;
    //         }
    //     }
    // } else {
    //     for (s32 i = 0; i < mWorldList->getWorldNum(); i++) {
    //         s32 worldId = calcWorldIdByOrderUnlock(i);
    //         if (worldId != -1) {
    //             if (i == idxPeach)
    //                 mWorldIdForWorldMap[0] = idxPeach;
    //             else {
    //                if (worldId < idxPeach)
    //                     worldId++;
    //                 mWorldIdForWorldMap[worldId] = i;
    //             }
    //         }
    //     }
    // }

    for (s32 i = 0; i < mWorldList->getWorldNum(); i++) {
        s32 worldId = calcWorldIdByOrderUnlock(i);
        if (worldId == -1) continue;

        if (!isUnlockPeach) {
            if (worldId == idxCloud && mUnlockWorldNum == idxCloud + 1)
                mWorldIdForWorldMap[idxCloud] = idxCity;
            else if (worldId == idxAttack && mUnlockWorldNum == idxAttack + 1)
                mWorldIdForWorldMap[idxAttack] = idxSky;
            else
                mWorldIdForWorldMap[i] = worldId;
        } else {
            if (i == idxPeach)
                mWorldIdForWorldMap[0] = idxPeach;
            else {
               if (worldId < idxPeach)
                    worldId++;
                mWorldIdForWorldMap[worldId] = i;
            }
        }
    }


    for (s32 i = 0; i < mWorldList->getWorldNum(); i++) {
        s32 worldId = calcWorldIdByOrderUnlock(i);
        if (worldId != -1) {
            mWorldIdForWorldWarpHole[i] = worldId;
            mWorldIdForShineList[i] = worldId;
        }
    }

    if (isUnlockPeach) {
        for (s32 i = 0; i < mWorldList->getWorldNum(); i++) {
            if (i == idxPeach)
                mWorldIdForShineList[0] = idxPeach;
            else if (i < idxPeach)
                mWorldIdForShineList[i+1] = mWorldIdForWorldWarpHole[i];
            else
                mWorldIdForShineList[i] = mWorldIdForWorldWarpHole[i];
        }
    }
}

void GameProgressData::checkAndChangeCorrectStatus(s32 param1, s32 param2) {
    if (param1 == GameDataFunction::getWorldIndexWaterfall() && param2 > 1)
        mWaterfallWorldProgress = 2;

    if (param1 > GameDataFunction::getWorldIndexWaterfall())
        mWaterfallWorldProgress = 2;

    if (param1 == GameDataFunction::getWorldIndexHat() && param2 > 1)
        mWaterfallWorldProgress = 2;

    if (param1 != GameDataFunction::getWorldIndexCloud() && isFindKoopa())
        mHomeStatus = 2;

    if (param1 != GameDataFunction::getWorldIndexBoss() && isBossAttackedHome())
        mHomeStatus = 5;
}

bool GameProgressData::isFindKoopa() const {
    return mHomeStatus == 3;
}

bool GameProgressData::isBossAttackedHome() const {
    return mHomeStatus == 6;
}

bool GameProgressData::isActivateHome() const {
    return mHomeStatus > 0;
}

void GameProgressData::activateHome() {
    if (mHomeStatus < 1)
        mHomeStatus = 1;
}

bool GameProgressData::isLaunchHome() const {
    return mHomeStatus > 1;
}

void GameProgressData::launchHome() {
    if (mHomeStatus < 2)
        mHomeStatus = 2;
}

void GameProgressData::findKoopa() {
    if (mHomeStatus < 3)
        mHomeStatus = 3;
}

bool GameProgressData::isCrashHome() const {
    return mHomeStatus == 4;
}

void GameProgressData::crashHome() {
    if (mHomeStatus < 4)
        mHomeStatus = 4;
}

bool GameProgressData::isRepairHome() const {
    return mHomeStatus > 4;
}

void GameProgressData::repairHome() {
    if (mHomeStatus < 5)
        mHomeStatus = 5;
}

void GameProgressData::bossAttackHome() {
    if (mHomeStatus < 6)
        mHomeStatus = 6;
}

bool GameProgressData::isRepairHomeByCrashedBoss() const {
    return mHomeStatus > 6;
}

void GameProgressData::repairHomeByCrashedBoss() {
    if (mHomeStatus < 7)
        mHomeStatus = 7;
}

s32 GameProgressData::getHomeLevel() const {
    return mHomeLevel;
}

void GameProgressData::upHomeLevel() {
    s32 nextHomeLevels[18] = { 0, 0, 1, 2, 3, 4, 4, 5, 6, 7, 8, 8, 9, 9, 9, 9, 9, 9 };
    s32 nextHomeLevel = nextHomeLevels[mUnlockWorldNum];
    nextHomeLevel = std::min(mHomeLevel + 1, nextHomeLevel);
    mHomeLevel = std::min(nextHomeLevel, 9);
}

s32 GameProgressData::getUnlockWorldNum() const {
    return mUnlockWorldNum;
}

bool GameProgressData::isUnlockWorld(s32 idx) const {
    return mIsUnlockWorld[idx];
}

s32 GameProgressData::getWorldIdForWorldMap(s32 idx) const {
    return mWorldIdForWorldMap[idx];
}

s32 GameProgressData::calcNextLockedWorldNumForWorldMap() const {
    if (mUnlockWorldNum == 3 || mUnlockWorldNum == 8)
        return 2;
    else
        return 1;
}

s32 GameProgressData::calcNextLockedWorldIdForWorldMap(s32 idx) const {
    switch (mUnlockWorldNum) {
    case 3:
        return idx == 0 ? GameDataFunction::getWorldIndexForest() : GameDataFunction::getWorldIndexLake();
    case 4:
        return isUnlockFirstForest() ? GameDataFunction::getWorldIndexLake() : GameDataFunction::getWorldIndexForest();
    case 5:
        return GameDataFunction::getWorldIndexCity();
    case 8:
        return idx != 0 ? GameDataFunction::getWorldIndexSnow() : GameDataFunction::getWorldIndexSea();
    case 9:
        return isUnlockFirstSea() ? GameDataFunction::getWorldIndexSea() : GameDataFunction::getWorldIndexSnow();
    case 11:
        return GameDataFunction::getWorldIndexSky();
    default:
        return mUnlockWorldNum;
    }
}

bool GameProgressData::isUnlockFirstForest() const {
    return (u32)mUnlockWorldStatusFirstBranch < 2;
}


bool GameProgressData::isUnlockFirstSea() const {
    if (!(mUnlockWorldStatusSecondBranch != 0 && mUnlockWorldStatusSecondBranch != 3))
        return true;

    return false;
}

s32 GameProgressData::getWorldIdForWorldWarpHole(s32 idx) const {
    return mWorldIdForWorldWarpHole[idx];
}

s32 GameProgressData::getWorldIdForShineList(s32 idx) const {
    return mWorldIdForShineList[idx];
}

s32 GameProgressData::calcWorldNumForShineList() const {
    s32 idxSpecial2 = GameDataFunction::getWorldIndexSpecial2();
    if (mIsUnlockWorld[idxSpecial2] && !mIsFirstTimeWorld[idxSpecial2])
        return mWorldList->getWorldNum();

    s32 worldNum = 0;
    for (s32 i = 0; i < mWorldList->getWorldNum(); i++) {
        worldNum += !mIsFirstTimeWorld[i];
    }

    return worldNum;
}

bool GameProgressData::isAlreadyGoWorld(s32 idx) const {
    return !mIsFirstTimeWorld[idx];
}

void GameProgressData::unlockNextWorld(s32 idx) {
    while (true) {
        updateList();
        if (mIsUnlockWorld[idx]) return;

        s32 idxForest = mWorldList->tryFindWorldIndexByDevelopName("Forest");
        s32 idxLake = mWorldList->tryFindWorldIndexByDevelopName("Lake");
        s32 idxSnow = mWorldList->tryFindWorldIndexByDevelopName("Snow");
        s32 idxSea = mWorldList->tryFindWorldIndexByDevelopName("Sea");

        if (idx == idxForest)
            unlockForest();
        else if (idx == idxLake)
            unlockLake();
        else if (idx == idxSnow)
            unlockSnow();
        else if (idx == idxSea)
            unlockSea();
        else
            unlockNormalWorld();

        updateList();

        s32 nextHomeLevels[17] = { 0, 0, 1, 2, 2, 4, 4, 4, 5, 5, 7, 8, 8, 9, 9, 9, 9 };

        mHomeLevel = mHomeLevel <= nextHomeLevels[idx] ? nextHomeLevels[idx] : mHomeLevel;

        if (mHomeLevel == 9)
            mHomeStatus = 7;

        if (mIsUnlockWorld[idx])
            return;
    }
}

void GameProgressData::unlockForest() {
    if (mUnlockWorldStatusFirstBranch == 0)
        mUnlockWorldStatusFirstBranch = 1;

    mUnlockWorldNum++;
}

void GameProgressData::unlockLake() {
    if (mUnlockWorldStatusFirstBranch == 0)
        mUnlockWorldStatusFirstBranch = 2;

    mUnlockWorldNum++;
}

void GameProgressData::unlockSnow() {
    if (mUnlockWorldStatusSecondBranch == 0)
        mUnlockWorldStatusSecondBranch = 4;

    mUnlockWorldNum++;
}

void GameProgressData::unlockSea() {
    if (mUnlockWorldStatusSecondBranch == 0)
        mUnlockWorldStatusSecondBranch = 3;

    mUnlockWorldNum++;
}

void GameProgressData::unlockNormalWorld() {
    mUnlockWorldNum++;
}

bool GameProgressData::isFirstTimeGoWorld(s32 idx) const {
    return mIsFirstTimeWorld[idx];
}

void GameProgressData::setAlreadyGoWorld(s32 idx) {
    mIsFirstTimeWorld[idx] = false;
}

bool GameProgressData::isTalkedCapNearHomeInWaterfall() const {
    return mWaterfallWorldProgress > 1;
}

void GameProgressData::talkCapNearHomeInWaterfall() {
    if (mWaterfallWorldProgress < 2)
        mWaterfallWorldProgress = 2;
}

void GameProgressData::write(al::ByamlWriter* writer) {    
    writer->pushHash("GameProgressData");
    writer->addInt("HomeStatus", mHomeStatus);
    writer->addInt("HomeLevel", mHomeLevel);
    writer->addInt("UnlockWorldNum", mUnlockWorldNum);
    writer->addInt("UnlockWorldStatusFirstBranch", mUnlockWorldStatusFirstBranch);
    writer->addInt("UnlockWorldStatusSecondBranch", mUnlockWorldStatusSecondBranch);
    writer->addInt("WaterfallWorldProgress", mWaterfallWorldProgress);

    writer->pushArray("IsFirstTimeWorld");
    for (s32 i = 0; i < mWorldList->getWorldNum(); i++)
        writer->addBool(mIsFirstTimeWorld[i]);
    writer->pop();

    writer->pop();
}

void GameProgressData::read(const al::ByamlIter& iter) {
    init();

    al::ByamlIter hash;
    iter.tryGetIterByKey(&hash, "GameProgressData");
    hash.tryGetIntByKey(&mHomeStatus, "HomeStatus");
    hash.tryGetIntByKey(&mHomeLevel, "HomeLevel");
    hash.tryGetIntByKey(&mUnlockWorldNum, "UnlockWorldNum");
    hash.tryGetIntByKey(&mUnlockWorldStatusFirstBranch, "UnlockWorldStatusFirstBranch");
    hash.tryGetIntByKey(&mUnlockWorldStatusSecondBranch, "UnlockWorldStatusSecondBranch");
    hash.tryGetIntByKey(&mWaterfallWorldProgress, "WaterfallWorldProgress");

    al::ByamlIter array;
    hash.tryGetIterByKey(&array, "IsFirstTimeWorld");
    for (s32 i = 0; i < mWorldList->getWorldNum(); i++)
        array.tryGetBoolByIndex(&mIsFirstTimeWorld[i], i);

    updateList();
}

void GameProgressData::initList() {
    for (s32 i = 0; i < mWorldList->getWorldNum(); i++) {
        mIsUnlockWorld[i] = false;
        mWorldIdForWorldMap[i] = -1;
        mWorldIdForWorldWarpHole[i] = -1;
        mWorldIdForShineList[i] = -1;
    }
}

s32 GameProgressData::calcWorldIdByOrderUnlock(s32 idx) const {
    s32 idxForest = mWorldList->tryFindWorldIndexByDevelopName("Forest");
    s32 idxLake = mWorldList->tryFindWorldIndexByDevelopName("Lake");
    s32 idxSnow = mWorldList->tryFindWorldIndexByDevelopName("Snow");
    s32 idxSea = mWorldList->tryFindWorldIndexByDevelopName("Sea");

    if (idx == idxForest)
        return (u32)mUnlockWorldStatusFirstBranch < 2 ? idx : idxLake;

    if (idx == idxLake)
        return (u32)mUnlockWorldStatusFirstBranch < 2 ? idx : idxForest;

    if (idx == idxSnow) {
        if (mUnlockWorldStatusSecondBranch != 3 && mUnlockWorldStatusSecondBranch != 0)
            return idxSea;
        else
            return idx;
    }
    if (idx == idxSea) {
        if (mUnlockWorldStatusSecondBranch != 3 && mUnlockWorldStatusSecondBranch != 0)
            return idxSnow;
        else
            return idx;
    }

    return idx;
}
