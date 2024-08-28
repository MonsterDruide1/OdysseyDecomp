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
    mHomeStatus = HomeStatus::None;
    mHomeLevel = 0;
    mUnlockWorldStatusFirstBranch = FirstBranch::None;
    mUnlockWorldStatusSecondBranch = SecondBranch::None;
    mUnlockWorldNum = 1;
    mWaterfallWorldProgress = WaterfallWorldProgress::None;

    for (s32 i = 0; i < mWorldList->getWorldNum(); i++)
        mIsFirstTimeWorld[i] = true;

    setAlreadyGoWorld(0);

    updateList();
}

void GameProgressData::updateList() {
    initList();

    s32 worldNum = mWorldList->getWorldNum();
    s32 idxPeach = mWorldList->tryFindWorldIndexByDevelopName("Peach");

    for (s32 j = 0, i = 0; i < worldNum; i++) {
        s32 worldId = calcWorldIdByOrderUnlock(i);
        if (worldId == -1)
            continue;

        mIsUnlockWorld[worldId] = true;
        if (++j == mUnlockWorldNum)
            break;
    }

    s32 idxCloud = mWorldList->tryFindWorldIndexByDevelopName("Cloud");
    s32 idxAttack = mWorldList->tryFindWorldIndexByDevelopName("Attack");
    s32 idxCity = mWorldList->tryFindWorldIndexByDevelopName("City");
    s32 idxSky = mWorldList->tryFindWorldIndexByDevelopName("Sky");

    bool isUnlockPeach = mIsUnlockWorld[idxPeach];

    for (s32 i = 0; i < worldNum; i++) {
        s32 worldId = calcWorldIdByOrderUnlock(i);
        if (worldId == -1)
            continue;

        if (isUnlockPeach) {
            if (i == idxPeach)
                mWorldIdForWorldMap[0] = idxPeach;
            else {
                if (worldId < idxPeach)
                    worldId++;
                mWorldIdForWorldMap[worldId] = i;
            }
        } else {
            if (worldId == idxCloud && mUnlockWorldNum == idxCloud + 1)
                mWorldIdForWorldMap[worldId] = idxCity;
            else if (worldId == idxAttack && mUnlockWorldNum == idxAttack + 1)
                mWorldIdForWorldMap[worldId] = idxSky;
            else
                mWorldIdForWorldMap[i] = worldId;
        }
    }

    for (s32 i = 0; i < worldNum; i++) {
        s32 worldId = calcWorldIdByOrderUnlock(i);
        if (worldId != -1) {
            mWorldIdForWorldWarpHole[i] = worldId;
            mWorldIdForShineList[i] = worldId;
        }
    }

    if (isUnlockPeach) {
        for (s32 i = 0; i < worldNum; i++)
            if (i == idxPeach)
                mWorldIdForShineList[0] = idxPeach;
            else if (i < idxPeach)
                mWorldIdForShineList[i + 1] = mWorldIdForWorldWarpHole[i];
            else
                mWorldIdForShineList[i] = mWorldIdForWorldWarpHole[i];
    }
}

void GameProgressData::checkAndChangeCorrectStatus(s32 worldId, s32 nextScenarioNo) {
    if (worldId == GameDataFunction::getWorldIndexWaterfall() && nextScenarioNo > 1)
        mWaterfallWorldProgress = WaterfallWorldProgress::TalkedCapNearHome;

    if (worldId > GameDataFunction::getWorldIndexWaterfall())
        mWaterfallWorldProgress = WaterfallWorldProgress::TalkedCapNearHome;

    if (worldId == GameDataFunction::getWorldIndexHat() && nextScenarioNo > 1)
        mWaterfallWorldProgress = WaterfallWorldProgress::TalkedCapNearHome;

    if (worldId != GameDataFunction::getWorldIndexCloud() && isFindKoopa())
        mHomeStatus = HomeStatus::LaunchedHome;

    if (worldId != GameDataFunction::getWorldIndexBoss() && isBossAttackedHome())
        mHomeStatus = HomeStatus::RepairedHome;
}

bool GameProgressData::isFindKoopa() const {
    return mHomeStatus == HomeStatus::FoundKoopa;
}

bool GameProgressData::isBossAttackedHome() const {
    return mHomeStatus == HomeStatus::BossAttackedHome;
}

bool GameProgressData::isActivateHome() const {
    return mHomeStatus > HomeStatus::None;
}

void GameProgressData::activateHome() {
    if (mHomeStatus < HomeStatus::ActivatedHome)
        mHomeStatus = HomeStatus::ActivatedHome;
}

bool GameProgressData::isLaunchHome() const {
    return mHomeStatus > HomeStatus::ActivatedHome;
}

void GameProgressData::launchHome() {
    if (mHomeStatus < HomeStatus::LaunchedHome)
        mHomeStatus = HomeStatus::LaunchedHome;
}

void GameProgressData::findKoopa() {
    if (mHomeStatus < HomeStatus::FoundKoopa)
        mHomeStatus = HomeStatus::FoundKoopa;
}

bool GameProgressData::isCrashHome() const {
    return mHomeStatus == HomeStatus::CrashedHome;
}

void GameProgressData::crashHome() {
    if (mHomeStatus < HomeStatus::CrashedHome)
        mHomeStatus = HomeStatus::CrashedHome;
}

bool GameProgressData::isRepairHome() const {
    return mHomeStatus > HomeStatus::CrashedHome;
}

void GameProgressData::repairHome() {
    if (mHomeStatus < HomeStatus::RepairedHome)
        mHomeStatus = HomeStatus::RepairedHome;
}

void GameProgressData::bossAttackHome() {
    if (mHomeStatus < HomeStatus::BossAttackedHome)
        mHomeStatus = HomeStatus::BossAttackedHome;
}

bool GameProgressData::isRepairHomeByCrashedBoss() const {
    return mHomeStatus > HomeStatus::BossAttackedHome;
}

void GameProgressData::repairHomeByCrashedBoss() {
    if (mHomeStatus < HomeStatus::RepairedHomeByCrashedBoss)
        mHomeStatus = HomeStatus::RepairedHomeByCrashedBoss;
}

s32 GameProgressData::getHomeLevel() const {
    return mHomeLevel;
}

void GameProgressData::upHomeLevel() {
    s32 nextHomeLevels[18] = {0, 0, 1, 2, 3, 4, 4, 5, 6, 7, 8, 8, 9, 9, 9, 9, 9, 9};
    s32 nextHomeLevel = nextHomeLevels[mUnlockWorldNum];
    nextHomeLevel = sead::Mathi::min(nextHomeLevel, mHomeLevel + 1);
    mHomeLevel = sead::Mathi::clampMax(nextHomeLevel, 9);
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
        return idx == 0 ? GameDataFunction::getWorldIndexForest() :
                          GameDataFunction::getWorldIndexLake();
    case 4:
        return isUnlockFirstForest() ? GameDataFunction::getWorldIndexLake() :
                                       GameDataFunction::getWorldIndexForest();
    case 5:
        return GameDataFunction::getWorldIndexCity();
    case 8:
        return idx != 0 ? GameDataFunction::getWorldIndexSnow() :
                          GameDataFunction::getWorldIndexSea();
    case 9:
        return isUnlockFirstSea() ? GameDataFunction::getWorldIndexSea() :
                                    GameDataFunction::getWorldIndexSnow();
    case 11:
        return GameDataFunction::getWorldIndexSky();
    default:
        return mUnlockWorldNum;
    }
}

bool GameProgressData::isUnlockFirstForest() const {
    return mUnlockWorldStatusFirstBranch < FirstBranch::Lake;
}

bool GameProgressData::isUnlockFirstSea() const {
    if (mUnlockWorldStatusSecondBranch == SecondBranch::None ||
        mUnlockWorldStatusSecondBranch == SecondBranch::Sea)
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
    if (isUnlockWorld(idxSpecial2) && isAlreadyGoWorld(idxSpecial2))
        return mWorldList->getWorldNum();

    s32 worldNum = 0;
    for (s32 i = 0; i < mWorldList->getWorldNum(); i++)
        worldNum += isAlreadyGoWorld(i);

    return worldNum;
}

bool GameProgressData::isAlreadyGoWorld(s32 idx) const {
    return !mIsFirstTimeWorld[idx];
}

void GameProgressData::unlockNextWorld(s32 idx) {
    while (true) {
        updateList();
        if (isUnlockWorld(idx))
            return;

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

        s32 nextHomeLevels[17] = {0, 0, 1, 2, 2, 4, 4, 4, 5, 5, 7, 8, 8, 9, 9, 9, 9};

        mHomeLevel = sead::Mathi::max(mHomeLevel, nextHomeLevels[idx]);

        if (mHomeLevel == 9)
            mHomeStatus = HomeStatus::RepairedHomeByCrashedBoss;

        if (isUnlockWorld(idx))
            return;
    }
}

void GameProgressData::unlockForest() {
    if (mUnlockWorldStatusFirstBranch == FirstBranch::None)
        mUnlockWorldStatusFirstBranch = FirstBranch::Forest;

    mUnlockWorldNum++;
}

void GameProgressData::unlockLake() {
    if (mUnlockWorldStatusFirstBranch == FirstBranch::None)
        mUnlockWorldStatusFirstBranch = FirstBranch::Lake;

    mUnlockWorldNum++;
}

void GameProgressData::unlockSnow() {
    if (mUnlockWorldStatusSecondBranch == SecondBranch::None)
        mUnlockWorldStatusSecondBranch = SecondBranch::Snow;

    mUnlockWorldNum++;
}

void GameProgressData::unlockSea() {
    if (mUnlockWorldStatusSecondBranch == SecondBranch::None)
        mUnlockWorldStatusSecondBranch = SecondBranch::Sea;

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
    return mWaterfallWorldProgress > WaterfallWorldProgress::GotFirstMoon;
}

void GameProgressData::talkCapNearHomeInWaterfall() {
    if (mWaterfallWorldProgress < WaterfallWorldProgress::TalkedCapNearHome)
        mWaterfallWorldProgress = WaterfallWorldProgress::TalkedCapNearHome;
}

void GameProgressData::write(al::ByamlWriter* writer) {
    writer->pushHash("GameProgressData");
    writer->addInt("HomeStatus", (s32)mHomeStatus);
    writer->addInt("HomeLevel", mHomeLevel);
    writer->addInt("UnlockWorldNum", mUnlockWorldNum);
    writer->addInt("UnlockWorldStatusFirstBranch", (s32)mUnlockWorldStatusFirstBranch);
    writer->addInt("UnlockWorldStatusSecondBranch", (s32)mUnlockWorldStatusSecondBranch);
    writer->addInt("WaterfallWorldProgress", (s32)mWaterfallWorldProgress);

    writer->pushArray("IsFirstTimeWorld");
    for (s32 i = 0; i < mWorldList->getWorldNum(); i++)
        writer->addBool(isFirstTimeGoWorld(i));
    writer->pop();

    writer->pop();
}

void GameProgressData::read(const al::ByamlIter& iter) {
    init();

    al::ByamlIter hash;
    iter.tryGetIterByKey(&hash, "GameProgressData");
    hash.tryGetIntByKey((s32*)&mHomeStatus, "HomeStatus");
    hash.tryGetIntByKey(&mHomeLevel, "HomeLevel");
    hash.tryGetIntByKey(&mUnlockWorldNum, "UnlockWorldNum");
    hash.tryGetIntByKey((s32*)&mUnlockWorldStatusFirstBranch, "UnlockWorldStatusFirstBranch");
    hash.tryGetIntByKey((s32*)&mUnlockWorldStatusSecondBranch, "UnlockWorldStatusSecondBranch");
    hash.tryGetIntByKey((s32*)&mWaterfallWorldProgress, "WaterfallWorldProgress");

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
        return mUnlockWorldStatusFirstBranch < FirstBranch::Lake ? idx : idxLake;

    if (idx == idxLake)
        return mUnlockWorldStatusFirstBranch < FirstBranch::Lake ? idx : idxForest;

    if (idx == idxSnow) {
        if (mUnlockWorldStatusSecondBranch != SecondBranch::None &&
            mUnlockWorldStatusSecondBranch != SecondBranch::Sea)
            return idxSea;
        else
            return idx;
    }
    if (idx == idxSea) {
        if (mUnlockWorldStatusSecondBranch != SecondBranch::None &&
            mUnlockWorldStatusSecondBranch != SecondBranch::Sea)
            return idxSnow;
        else
            return idx;
    }

    return idx;
}
