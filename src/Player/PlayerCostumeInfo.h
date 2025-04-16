#pragma once

struct PlayerHeadCostumeInfo {
public:
    PlayerHeadCostumeInfo(const char*);

    const char* costumeName;
    bool isFullFace = false;
    bool isShrinkNose = false;
    bool isPreventHead = false;
    bool isEnableBigEar = false;
    bool isEnableHairNoCap = false;
    bool isMario64 = false;
    bool isHaveShort = false;
    bool isHideBeard = false;
    bool isHideEarringPeach = false;
    bool isHideEarringLink = false;
    bool isUseStrap = false;
    bool isInvisibleHead = false;
};

struct PlayerBodyCostumeInfo {
public:
    PlayerBodyCostumeInfo(const char*);

    const char* costumeName;
    s32 warmLevel = 0;
    bool isIgnoreTemperature = false;
    bool isUseHeadSuffix = false;
    bool isBigEar = false;
    bool isHideHeadHair = false;
    bool isUseBodyHair = false;
    bool isExistHairNoCap = false;
    bool isUseShortHead = false;
    bool isNoPairHead = false;
    bool isMario64 = false;
    bool isHidePainNose = false;
    bool isUseBeard = false;
    bool isUseEarringPeach = false;
    bool isUseEarringLink = false;
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
