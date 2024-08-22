#pragma once

#include <basis/seadTypes.h>

#include "System/ByamlSave.h"

class WorldList;

class GameProgressData : public ByamlSave {
public:
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& iter) override;

    GameProgressData(const WorldList* worldList);
    void init();
    void updateList();
    void checkAndChangeCorrectStatus(s32 worldId, s32 nextScenarioNo);
    bool isFindKoopa() const;
    bool isBossAttackedHome() const;
    bool isActivateHome() const;
    void activateHome();
    bool isLaunchHome() const;
    void launchHome();
    void findKoopa();
    bool isCrashHome() const;
    void crashHome();
    bool isRepairHome() const;
    void repairHome();
    void bossAttackHome();
    bool isRepairHomeByCrashedBoss() const;
    void repairHomeByCrashedBoss();
    s32 getHomeLevel() const;
    void upHomeLevel();
    s32 getUnlockWorldNum() const;
    bool isUnlockWorld(s32 idx) const;
    s32 getWorldIdForWorldMap(s32 idx) const;
    s32 calcNextLockedWorldNumForWorldMap() const;
    s32 calcNextLockedWorldIdForWorldMap(s32 idx) const;
    bool isUnlockFirstForest() const;
    bool isUnlockFirstSea() const;
    s32 getWorldIdForWorldWarpHole(s32 idx) const;
    s32 getWorldIdForShineList(s32 idx) const;
    s32 calcWorldNumForShineList() const;
    bool isAlreadyGoWorld(s32 idx) const;
    void unlockNextWorld(s32 idx);
    void unlockForest();
    void unlockLake();
    void unlockSnow();
    void unlockSea();
    void unlockNormalWorld();
    bool isFirstTimeGoWorld(s32 idx) const;
    void setAlreadyGoWorld(s32 idx);
    bool isTalkedCapNearHomeInWaterfall() const;
    void talkCapNearHomeInWaterfall();
    void initList();
    s32 calcWorldIdByOrderUnlock(s32 idx) const;

private:
    s32* mWorldIdForWorldMap = nullptr;
    s32* mWorldIdForWorldWarpHole = nullptr;
    s32* mWorldIdForShineList = nullptr;
    bool* mIsUnlockWorld = nullptr;
    s32 mUnlockWorldNum = 1;
    s32 mUnlockWorldStatusFirstBranch = 0;
    s32 mUnlockWorldStatusSecondBranch = 0;
    s32 mHomeStatus = 0;
    s32 mHomeLevel = 0;
    const WorldList* mWorldList = nullptr;
    bool* mIsFirstTimeWorld = nullptr;
    s32 mWaterfallWorldProgress = 0;
};
