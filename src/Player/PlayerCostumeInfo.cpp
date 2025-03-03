#include "Player/PlayerCostumeInfo.h"

#include "Library/Base/StringUtil.h"
#include "Library/Math/MathUtil.h"

PlayerHeadCostumeInfo::PlayerHeadCostumeInfo(const char* a1) {
    costumeName = a1;
}

PlayerBodyCostumeInfo::PlayerBodyCostumeInfo(const char* a1) {
    costumeName = a1;
}

PlayerCostumeInfo::PlayerCostumeInfo() = default;

void PlayerCostumeInfo::init(const PlayerBodyCostumeInfo* body, const PlayerHeadCostumeInfo* head) {
    mBodyInfo = body;
    mHeadInfo = head;
}

bool PlayerCostumeInfo::isEnableBigEar() const {
    return mBodyInfo->isBigEar && mHeadInfo->isEnableBigEar;
}

bool PlayerCostumeInfo::isEnableHairNoCap() const {
    return mHeadInfo->isEnableHairNoCap &&
           (mBodyInfo->isUseBodyHair && mBodyInfo->isExistHairNoCap);
}

bool PlayerCostumeInfo::isEnableCostume2D() const {
    if (mBodyInfo->isNoPairHead)
        return al::isEqualString(mHeadInfo->costumeName, "Mario");
    return al::isEqualString(mBodyInfo->costumeName, mHeadInfo->costumeName);
}

bool PlayerCostumeInfo::isNeedShrinkNose() const {
    return mHeadInfo->isShrinkNose;
}

bool PlayerCostumeInfo::isNeedBodyHair() const {
    if (!mBodyInfo->isUseBodyHair || (mBodyInfo->isMario64 && mHeadInfo->isMario64))
        return false;
    return true;
}

bool PlayerCostumeInfo::isNeedSyncBodyHair() const {
#ifdef MATCHING_HACK_NX_CLANG
    s64 v1 = *(s64*)&mBodyInfo->isUseBodyHair;
    if (!((char)v1))
        return false;
    if ((v1 & 0xFF00000000LL) && mHeadInfo->isMario64)
        return false;
    return !mBodyInfo->isMario64;
#else
    if (!mBodyInfo->isUseBodyHair)
        return false;
    if (mBodyInfo->isMario64 && mHeadInfo->isMario64)
        return false;
    return !mBodyInfo->isMario64;
#endif
}

bool PlayerCostumeInfo::isNeedFullFaceAnim() const {
    return mHeadInfo->isFullFace;
}

bool PlayerCostumeInfo::isHidePainNose() const {
    return mBodyInfo->isHidePainNose || mHeadInfo->isInvisibleHead;
}

bool PlayerCostumeInfo::isEnableEarring() const {
    if (mBodyInfo->isUseEarringPeach)
        return !mHeadInfo->isHideEarringPeach;
    return !mBodyInfo->isUseEarringLink || !mHeadInfo->isHideEarringLink;
}

bool PlayerCostumeInfo::isSyncFaceBeard() const {
    return mBodyInfo->isUseBeard && mHeadInfo->isHideBeard;
}

bool PlayerCostumeInfo::isSyncStrap() const {
    return mHeadInfo->isUseStrap && !mBodyInfo->isUseBeard;
}

bool PlayerCostumeInfo::isFollowJoeStrap() const {
    return mHeadInfo->isUseStrap && !mBodyInfo->isMario64;
}

bool PlayerCostumeInfo::isPreventHeadPain() const {
    return mHeadInfo->isPreventHead;
}

bool PlayerCostumeInfo::isInvisibleHead() const {
    return mHeadInfo->isInvisibleHead;
}

s32 PlayerCostumeInfo::calcWarmLevel(s32 baseLevel) const {
    if (mBodyInfo->isIgnoreTemperature)
        return false;
    return al::clamp(mBodyInfo->warmLevel + baseLevel, -3, 3);
}
