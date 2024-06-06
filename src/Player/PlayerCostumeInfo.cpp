#include "Player/PlayerCostumeInfo.h"

#include "Library/Base/StringUtil.h"
#include "Library/Math/MathUtil.h"

PlayerHeadCostumeInfo::PlayerHeadCostumeInfo(char const* a1) {
    _0 = a1;
}
PlayerBodyCostumeInfo::PlayerBodyCostumeInfo(char const* a1) {
    _0 = a1;
}

PlayerCostumeInfo::PlayerCostumeInfo() = default;

void PlayerCostumeInfo::init(const PlayerBodyCostumeInfo* body, const PlayerHeadCostumeInfo* head) {
    mPlayerBodyCostumeInfo = body;
    mPlayerHeadCostumeInfo = head;
}

bool PlayerCostumeInfo::isEnableBigEar() const {
    return mPlayerBodyCostumeInfo->mIsBigEar && mPlayerHeadCostumeInfo->mIsEnableBigEar;
}

bool PlayerCostumeInfo::isEnableHairNoCap() const {
    return mPlayerHeadCostumeInfo->mIsEnableHairNoCap &&
           (mPlayerBodyCostumeInfo->mIsUseBodyHair && mPlayerBodyCostumeInfo->mIsExistHairNoCap);
}

bool PlayerCostumeInfo::isEnableCostume2D() const {
    if (mPlayerBodyCostumeInfo->mIsNoPairHead)
        return al::isEqualString(mPlayerHeadCostumeInfo->_0, "Mario");
    return al::isEqualString(mPlayerBodyCostumeInfo->_0, mPlayerHeadCostumeInfo->_0);
}

bool PlayerCostumeInfo::isNeedShrinkNose() const {
    return mPlayerHeadCostumeInfo->mIsShrinkNose;
}

bool PlayerCostumeInfo::isNeedBodyHair() const {
    if (!mPlayerBodyCostumeInfo->mIsUseBodyHair ||
        (mPlayerBodyCostumeInfo->mIsMario64 && mPlayerHeadCostumeInfo->mIsMario64))
        return false;
    return true;
}

bool PlayerCostumeInfo::isNeedSyncBodyHair() const {
#ifdef MATCHING_HACK_NX_CLANG
    s64 v1 = *(s64*)&mPlayerBodyCostumeInfo->mIsUseBodyHair;
    if (!((char)v1))
        return false;
    if ((v1 & 0xFF00000000LL) && mPlayerHeadCostumeInfo->mIsMario64)
        return false;
    return !mPlayerBodyCostumeInfo->mIsMario64;
#else
    if (!mPlayerBodyCostumeInfo->mIsUseBodyHair)
        return false;
    if (mPlayerBodyCostumeInfo->mIsMario64 && mPlayerHeadCostumeInfo->mIsMario64)
        return false;
    return !mPlayerBodyCostumeInfo->mIsMario64;
#endif
}

bool PlayerCostumeInfo::isNeedFullFaceAnim() const {
    return mPlayerHeadCostumeInfo->mIsFullFace;
}

bool PlayerCostumeInfo::isHidePainNose() const {
    return mPlayerBodyCostumeInfo->mIsHidePainNose || mPlayerHeadCostumeInfo->mIsInvisibleHead;
}

bool PlayerCostumeInfo::isEnableEarring() const {
    if (mPlayerBodyCostumeInfo->mIsUseEarringPeach)
        return !mPlayerHeadCostumeInfo->mIsHideEarringPeach;
    return !mPlayerBodyCostumeInfo->mIsUseEarringLink ||
           !mPlayerHeadCostumeInfo->mIsHideEarringLink;
}

bool PlayerCostumeInfo::isSyncFaceBeard() const {
    return mPlayerBodyCostumeInfo->mIsUseBeard && mPlayerHeadCostumeInfo->mIsHideBeard;
}

bool PlayerCostumeInfo::isSyncStrap() const {
    return mPlayerHeadCostumeInfo->mIsUseStrap && !mPlayerBodyCostumeInfo->mIsUseBeard;
}

bool PlayerCostumeInfo::isFollowJoeStrap() const {
    return mPlayerHeadCostumeInfo->mIsUseStrap && !mPlayerBodyCostumeInfo->mIsMario64;
}

bool PlayerCostumeInfo::isPreventHeadPain() const {
    return mPlayerHeadCostumeInfo->mIsPreventHead;
}

bool PlayerCostumeInfo::isInvisibleHead() const {
    return mPlayerHeadCostumeInfo->mIsInvisibleHead;
}

s32 PlayerCostumeInfo::calcWarmLevel(s32 baseLevel) const {
    if (mPlayerBodyCostumeInfo->mIsIgnoreTemperature)
        return false;
    return al::clamp(mPlayerBodyCostumeInfo->mWarmLevel + baseLevel, -3, 3);
}
