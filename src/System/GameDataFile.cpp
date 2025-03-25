#include "System/GameDataFile.h"

#include "Library/Base/StringUtil.h"

#include "System/GameDataFunction.h"
#include "System/GameProgressData.h"

// NON_MATCHING
void GameDataFile::HintInfo::clear() {
    mStageName.clear();
    mObjId.clear();
    //    mScenarioName.clear();
    mObjectName = nullptr;
    mTrans = sead::Vector3f::zero;
    mTransAgain = sead::Vector3f::zero;
    mMainScenarioNo = -1;
    mWorldIndex = -1;
    mUniqueID = -1;
    mIsMoonRock = false;
    unkBool1 = false;
    unkPtr6 = nullptr;
    mHintStatus = HintStatus::NONE;
    mIsAchievement = false;
    mIsGrand = false;
    unkInt3 = 0;
    mIsShopMoon = false;
    mHintIdx = 0;

    mOptionalID.clear();
    mProgressBitflag = 0;
    unkBool2 = 0;
    unkBool3 = 0;
}

bool GameDataFile::HintInfo::isDisableByWorldWarpHole(bool condition) const {
    if (!condition && al::isEqualString(mOptionalID.cstr(), "WorldWarpHoleShine"))
        return true;
    return false;
}

bool GameDataFile::HintInfo::isEnableUnlock(s32 curWorldId, bool isGameClear, s32 scenarioNo,
                                            bool isInWorld) const {
    if (testFunc(curWorldId, isGameClear, scenarioNo, isInWorld)) {
        if (unkBool1)
            return false;

        if (mHintStatus == HintStatus::NONE)
            return true;

        if (isGameClear && mIsMoonRock)
            return false;
        else if (!isGameClear && mIsMoonRock)
            return true;
    }

    return false;
}

bool GameDataFile::HintInfo::isHintStatusUnlock(s32 curWorldId, s32 scenarioNo,
                                                bool isInWorld) const {
    if (mWorldIndex != curWorldId || unkBool1 || mHintStatus != HintStatus::NONE ||
        (isInWorld ? !mIsMoonRock : mIsMoonRock))
        return false;

    return !GameDataFunction::isCityWorldCeremonyAll(curWorldId, scenarioNo) || unkBool3;
}

bool GameDataFile::HintInfo::isHintStatusUnlockByNpc() const {
    return mHintStatus == HintStatus::NPC && !mIsMoonRock;
}

bool GameDataFile::HintInfo::isHintStatusUnlockByAmiibo() const {
    return mHintStatus == HintStatus::AMIIBO;
}

bool GameDataFile::HintInfo::isEnableNameUnlockByScenario(s32 curWorldId, s32 scenarioNo,
                                                          bool isInWorld) const {
    if (isDisableByWorldWarpHole(isInWorld))
        return false;
    if (mWorldIndex != curWorldId || unkBool2)
        return false;
    if (mProgressBitflag.countOnBit())
        return mProgressBitflag.isOnBit(scenarioNo - 1);
    return true;
}

bool GameDataFile::HintInfo::testFunc(s32 curWorldId, bool isGameClear, s32 scenarioNo,
                                      bool isInWorld) const {
    if (isDisableByWorldWarpHole(isInWorld))
        return false;
    if (mWorldIndex != curWorldId || unkBool2 || isGameClear)
        return false;
    if (mProgressBitflag.countOnBit())
        return mProgressBitflag.isOnBit(scenarioNo - 1);
    return true;
}

bool GameDataFile::isGameClear() const {
    return isUnlockedWorld(GameDataFunction::getWorldIndexPeach()) &&
           isAlreadyGoWorld(GameDataFunction::getWorldIndexPeach());
}

bool GameDataFile::isUnlockedWorld(s32 index) const {
    return mGameProgressData->isUnlockWorld(index);
}

bool GameDataFile::isAlreadyGoWorld(s32 index) const {
    return mGameProgressData->isAlreadyGoWorld(index);
}
