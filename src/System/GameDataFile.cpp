#include "System/GameDataFile.h"

#include <numeric>

#include "Library/Placement/PlacementId.h"

#include "System/GameDataFunction.h"
#include "System/GameProgressData.h"
#include "System/SphinxQuizData.h"
#include "System/WorldList.h"
#include "Util/ScenePrepoFunction.h"

// NON_MATCHING: https://decomp.me/scratch/jv20V
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
    mAchievementStatus = AchievementStatus::None;
    mIsShopMoon = false;
    mHintIdx = 0;

    mOptionalID.clear();
    mProgressBitflag = 0;
    mIsDisabled = false;
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
    if (mWorldIndex != curWorldId || mIsDisabled)
        return false;
    if (mProgressBitflag.countOnBit())
        return mProgressBitflag.isOnBit(scenarioNo - 1);
    return true;
}

bool GameDataFile::HintInfo::testFunc(s32 curWorldId, bool isGameClear, s32 scenarioNo,
                                      bool isInWorld) const {
    if (isDisableByWorldWarpHole(isInWorld))
        return false;
    if (mWorldIndex != curWorldId || mIsDisabled || isGameClear)
        return false;
    if (mProgressBitflag.countOnBit())
        return mProgressBitflag.isOnBit(scenarioNo - 1);
    return true;
}

void GameDataFile::unlockAchievementShineName() {
    for (s32 i = 0; i < mHintList.size(); i++)
        if (mHintList[i].mAchievementStatus == AchievementStatus::None &&
            mHintList[i].mIsAchievement)
            mHintList[i].mAchievementStatus = AchievementStatus::Unlocked;
    mHasUnlockedAchievementNames = true;
}

bool GameDataFile::isKidsMode() const {
    return mIsAssistMode;
}

void GameDataFile::updateWorldMapIndex() {
    mWorldMapNum = 0;
    for (s32 i = 0; i < mWorldMapIndex.size(); i++)
        mWorldMapIndex[i] = -1;

    bool unlockedMushroom = isUnlockedWorld(GameDataFunction::getWorldIndexPeach());
    if (unlockedMushroom) {
        mWorldMapIndex[0] = GameDataFunction::getWorldIndexPeach();
        mWorldMapNum++;
    }

    for (s32 i = 0; i < mGameDataHolder->getWorldList()->getWorldNum(); i++) {
        s32 index = mUnlockedWorldId[i];
        if (i == GameDataFunction::getWorldIndexPeach() || index < 0)
            continue;
        if (unlockedMushroom && index < GameDataFunction::getWorldIndexPeach())
            index++;
        mWorldMapIndex[index] = i;
        mWorldMapNum++;
    }
}

void GameDataFile::updateWorldWarpIndex() {
    mWorldWarpNum = 0;
    for (s32 i = 0; i < mWorldWarpIndex.size(); i++)
        mWorldWarpIndex[i] = -1;
    for (s32 j = 0; j < mGameDataHolder->getWorldList()->getWorldNum(); j++) {
        if (mUnlockedWorldId[j] >= 0) {
            mWorldWarpIndex[mUnlockedWorldId[j]] = j;
            mWorldWarpNum++;
        }
    }
}

s32 GameDataFile::getScenarioNo(s32 worldId) const {
    return mScenarioNo[worldId];
}

s32 GameDataFile::getMainScenarioNo(s32 worldId) const {
    return mMainScenarioNo[worldId];
}

bool GameDataFile::isEmpty() const {
    return mIsEmpty;
}

bool GameDataFile::isGameClear() const {
    return isUnlockedWorld(GameDataFunction::getWorldIndexPeach()) &&
           isAlreadyGoWorld(GameDataFunction::getWorldIndexPeach());
}

bool GameDataFile::isUnlockedWorld(s32 worldId) const {
    return mGameProgressData->isUnlockWorld(worldId);
}

bool GameDataFile::isAlreadyGoWorld(s32 worldId) const {
    return mGameProgressData->isAlreadyGoWorld(worldId);
}

void GameDataFile::updateSaveTimeForDisp() {
    mSaveTimeForDisp = mSaveTime;
}

sead::DateTime GameDataFile::getLastUpdateTime() const {
    return mSaveTimeForDisp;
}

void GameDataFile::generateSaveDataIdForPrepoForWrite() {
    mSaveDataIdForPrepoForWrite = rs::prepo::generateSaveDataId();
}

bool GameDataFile::checkIsHomeStage(const char* stageName) const {
    return mGameDataHolder->getWorldList()->tryFindWorldIndexByMainStageName(stageName) != -1;
}

const char* GameDataFile::getStageNameCurrent() const {
    return mCurrentStageName.cstr();
}

const char* GameDataFile::tryGetStageNameCurrent() const {
    if (mCurrentStageName.isEmpty())
        return nullptr;
    const char* stage = mCurrentStageName.cstr();
    if (al::isEqualString(stage, ""))
        return nullptr;
    return stage;
}

const char* GameDataFile::getStageNameNext() const {
    if (mNextStageName.isEmpty())
        return nullptr;
    const char* stage = mNextStageName.cstr();
    if (al::isEqualString(stage, ""))
        return nullptr;
    return stage;
}

s32 GameDataFile::getTotalShopShineNum() const {
    return std::accumulate(mShopShineNum.begin(), mShopShineNum.end(), 0);
}

s32 GameDataFile::getPayShineNum(s32 worldId) const {
    if (worldId == -1)
        worldId = getCurrentWorldIdNoDevelop();
    return mPayShineNum[worldId];
}

bool GameDataFile::isStartedObj(const char* stageName, const char* objId) const {
    for (s32 i = 0; i < mStartedObj.size(); i++)
        if (mStartedObj[i].isEqual(stageName, objId))
            return true;
    return false;
}

const GameDataFile::SaveObjS32*
GameDataFile::tryFindSaveObjS32(const al::PlacementId* placementId) {
    const char* stageNameString = mCurrentStageName.cstr();
    SaveObjS32* buffer = mSaveObjS32.begin();
    sead::FixedSafeString<128> objId;
    placementId->makeString(&objId);
    sead::FixedSafeString<128> unusedString;
    for (s32 i = 0; i < mSaveObjS32.size(); i++) {
        SaveObjS32* item = &buffer[i];
        if (item->objInfo.isEqual(stageNameString, objId.cstr()))
            return item;
    }
    return nullptr;
}

void GameDataFile::useCoinCollect(s32 purplesUsed) {
    mUseCoinCollectNum[getCurrentWorldIdNoDevelop()] += purplesUsed;
    mGameDataHolder->setRequireSave();
}

void GameDataFile::answerCorrectSphinxQuiz() {
    mSphinxQuizData->answerCorrectSphinxQuiz(getCurrentWorldIdNoDevelop());
    mGameDataHolder->setRequireSave();
}

void GameDataFile::answerCorrectSphinxQuizAll() {
    mSphinxQuizData->answerCorrectSphinxQuizAll(getCurrentWorldIdNoDevelop());
    mGameDataHolder->setRequireSave();
}

bool GameDataFile::isAnswerCorrectSphinxQuiz(s32 index) const {
    return mSphinxQuizData->isAnswerCorrectSphinxQuiz(index);
}

bool GameDataFile::isAnswerCorrectSphinxQuizAll(s32 index) const {
    return mSphinxQuizData->isAnswerCorrectSphinxQuizAll(index);
}

bool GameDataFile::isTalkAlreadyLocalLanguage() const {
    return mIsTalkLocalLanguage[getCurrentWorldIdNoDevelop()];
}

s32 GameDataFile::calcGetCheckpointNum() const {
    s32 count = 0;
    for (s32 i = 0; i < mGotCheckpoint.size(); i++)
        count += !mGotCheckpoint[i].isEmpty();
    return count;
}

s32 GameDataFile::calcWorldWarpHoleThroughNum() const {
    return std::accumulate(mIsWorldWarpHoleThrough.begin(), mIsWorldWarpHoleThrough.end(), 0);
}

void GameDataFile::wearCostume(const char* name) {
    mCurrentCostumeName.format("%s", name);
}

void GameDataFile::wearCap(const char* name) {
    mCurrentCapName.format("%s", name);
}

void GameDataFile::enableHintById(s32 shineIndex) {
    if (shineIndex >= 0)
        mHintList[shineIndex].mIsDisabled = false;
}

inline s32 GameDataFile::getCurrentWorldIdNoDevelop() const {
    return sead::Mathi::max(mCurrentWorldId, 0);
}
