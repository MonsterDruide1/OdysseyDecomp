#pragma once

#include <basis/seadTypes.h>

struct PlayerHeadCostumeInfo {
public:
    PlayerHeadCostumeInfo(const char*);

    const char* mCostumeName;
    bool mIsFullFace = false;
    bool mIsShrinkNose = false;
    bool mIsPreventHead = false;
    bool mIsEnableBigEar = false;
    bool mIsEnableHairNoCap = false;
    bool mIsMario64 = false;
    bool mIsHaveShort = false;
    bool mIsHideBeard = false;
    bool mIsHideEarringPeach = false;
    bool mIsHideEarringLink = false;
    bool mIsUseStrap = false;
    bool mIsInvisibleHead = false;
};

struct PlayerBodyCostumeInfo {
public:
    PlayerBodyCostumeInfo(const char*);

    const char* mCostumeName;
    s32 mWarmLevel = 0;
    bool mIsIgnoreTemperature = false;
    bool mIsUseHeadSuffix = false;
    bool mIsBigEar = false;
    bool mIsHideHeadHair = false;
    bool mIsUseBodyHair = false;
    bool mIsExistHairNoCap = false;
    bool mIsUseShortHead = false;
    bool mIsNoPairHead = false;
    bool mIsMario64 = false;
    bool mIsHidePainNose = false;
    bool mIsUseBeard = false;
    bool mIsUseEarringPeach = false;
    bool mIsUseEarringLink = false;
};

class PlayerCostumeInfo {
public:
    PlayerCostumeInfo();
    void init(const PlayerBodyCostumeInfo*, const PlayerHeadCostumeInfo*);
    bool isEnableBigEar() const;
    bool isEnableHairNoCap() const;
    bool isEnableCostume2D() const;
    bool isNeedShrinkNose() const;
    bool isNeedBodyHair() const;
    bool isNeedSyncBodyHair() const;
    bool isNeedFullFaceAnim() const;
    bool isHidePainNose() const;
    bool isEnableEarring() const;
    bool isSyncFaceBeard() const;
    bool isSyncStrap() const;
    bool isFollowJoeStrap() const;
    bool isPreventHeadPain() const;
    bool isInvisibleHead() const;
    s32 calcWarmLevel(s32) const;

private:
    const PlayerBodyCostumeInfo* mBodyInfo = nullptr;
    const PlayerHeadCostumeInfo* mHeadInfo = nullptr;
};
