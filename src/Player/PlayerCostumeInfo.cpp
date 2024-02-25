#include "Player/PlayerCostumeInfo.h"

#include "Library/Base/String.h"
#include "Library/Math/MathUtil.h"

PlayerHeadCostumeInfo::PlayerHeadCostumeInfo(char const* a1) {
    mCostumeName = a1;
}
PlayerBodyCostumeInfo::PlayerBodyCostumeInfo(char const* a1) {
    mCostumeName = a1;
}

PlayerCostumeInfo::PlayerCostumeInfo() = default;

void PlayerCostumeInfo::init(const PlayerBodyCostumeInfo* body, const PlayerHeadCostumeInfo* head) {
    mBodyInfo = body;
    mHeadInfo = head;
}

bool PlayerCostumeInfo::isEnableBigEar() const {
    return mBodyInfo->mIsBigEar && mHeadInfo->mIsEnableBigEar;
}

bool PlayerCostumeInfo::isEnableHairNoCap() const {
    return mHeadInfo->mIsEnableHairNoCap &&
           (mBodyInfo->mIsUseBodyHair && mBodyInfo->mIsExistHairNoCap);
}

bool PlayerCostumeInfo::isEnableCostume2D() const {
    if (mBodyInfo->mIsNoPairHead)
        return al::isEqualString(mHeadInfo->mCostumeName, "Mario");
    return al::isEqualString(mBodyInfo->mCostumeName, mHeadInfo->mCostumeName);
}

bool PlayerCostumeInfo::isNeedShrinkNose() const {
    return mHeadInfo->mIsShrinkNose;
}

bool PlayerCostumeInfo::isNeedBodyHair() const {
    if (!mBodyInfo->mIsUseBodyHair || (mBodyInfo->mIsMario64 && mHeadInfo->mIsMario64))
        return false;
    return true;
}

bool PlayerCostumeInfo::isNeedSyncBodyHair() const {
#ifdef MATCHING_HACK_NX_CLANG
    long v1 = *(long*)&mBodyInfo->mIsUseBodyHair;
    if (!((char)v1))
        return false;
    if ((v1 & 0xFF00000000LL) && mHeadInfo->mIsMario64)
        return false;
    return !mBodyInfo->mIsMario64;
#else
    if (!mBodyInfo->mIsUseBodyHair)
        return false;
    if (mBodyInfo->mIsMario64 && mHeadInfo->mIsMario64)
        return false;
    return !mBodyInfo->mIsMario64;
#endif
}

bool PlayerCostumeInfo::isNeedFullFaceAnim() const {
    return mHeadInfo->mIsFullFace;
}

bool PlayerCostumeInfo::isHidePainNose() const {
    return mBodyInfo->mIsHidePainNose || mHeadInfo->mIsInvisibleHead;
}

bool PlayerCostumeInfo::isEnableEarring() const {
    if (mBodyInfo->mIsUseEarringPeach)
        return !mHeadInfo->mIsHideEarringPeach;
    return !mBodyInfo->mIsUseEarringLink || !mHeadInfo->mIsHideEarringLink;
}

bool PlayerCostumeInfo::isSyncFaceBeard() const {
    return mBodyInfo->mIsUseBeard && mHeadInfo->mIsHideBeard;
}

bool PlayerCostumeInfo::isSyncStrap() const {
    return mHeadInfo->mIsUseStrap && !mBodyInfo->mIsUseBeard;
}

bool PlayerCostumeInfo::isFollowJoeStrap() const {
    return mHeadInfo->mIsUseStrap && !mBodyInfo->mIsMario64;
}

bool PlayerCostumeInfo::isPreventHeadPain() const {
    return mHeadInfo->mIsPreventHead;
}

bool PlayerCostumeInfo::isInvisibleHead() const {
    return mHeadInfo->mIsInvisibleHead;
}

s32 PlayerCostumeInfo::calcWarmLevel(s32 baseLevel) const {
    if (mBodyInfo->mIsIgnoreTemperature)
        return false;
    return al::clamp(mBodyInfo->mWarmLevel + baseLevel, -3, 3);
}
