#include "System/GameDataFile.h"

#include "Library/Placement/PlacementId.h"

#include "System/GameDataFunction.h"
#include "System/GameProgressData.h"
#include "System/SphinxQuizData.h"
#include "System/WorldList.h"
#include "Util/ScenePrepoFunction.h"

// NON_MATCHING: https://decomp.me/scratch/jv20V
void GameDataFile::HintInfo::clear() {
    stageName.clear();
    objId.clear();
    //    mScenarioName.clear();
    objectName = nullptr;
    trans = sead::Vector3f::zero;
    transAgain = sead::Vector3f::zero;
    mainScenarioNo = -1;
    worldIndex = -1;
    uniqueID = -1;
    isMoonRock = false;
    unkBool1 = false;
    unkPtr6 = nullptr;
    hintStatus = HintStatus::None;
    isAchievement = false;
    isGrand = false;
    achievementStatus = AchievementStatus::None;
    isShopMoon = false;
    hintIdx = 0;

    optionalID.clear();
    progressBitflag = 0;
    isDisabled = false;
    unkBool3 = 0;
}

bool GameDataFile::HintInfo::isDisableByWorldWarpHole(bool condition) const {
    if (!condition && al::isEqualString(optionalID.cstr(), "WorldWarpHoleShine"))
        return true;
    return false;
}

bool GameDataFile::HintInfo::isEnableUnlock(s32 curWorldId, bool isGameClear, s32 scenarioNo,
                                            bool isInWorld) const {
    if (testFunc(curWorldId, isGameClear, scenarioNo, isInWorld)) {
        if (unkBool1)
            return false;

        if (hintStatus == HintStatus::None)
            return true;

        if (isGameClear && isMoonRock)
            return false;
        else if (!isGameClear && isMoonRock)
            return true;
    }

    return false;
}

bool GameDataFile::HintInfo::isHintStatusUnlock(s32 curWorldId, s32 scenarioNo,
                                                bool isInWorld) const {
    if (worldIndex != curWorldId || unkBool1 || hintStatus != HintStatus::None ||
        (isInWorld ? !isMoonRock : isMoonRock))
        return false;

    return !GameDataFunction::isCityWorldCeremonyAll(curWorldId, scenarioNo) || unkBool3;
}

bool GameDataFile::HintInfo::isHintStatusUnlockByNpc() const {
    return hintStatus == HintStatus::Npc && !isMoonRock;
}

bool GameDataFile::HintInfo::isHintStatusUnlockByAmiibo() const {
    return hintStatus == HintStatus::Amiibo;
}

bool GameDataFile::HintInfo::isEnableNameUnlockByScenario(s32 curWorldId, s32 scenarioNo,
                                                          bool isInWorld) const {
    if (isDisableByWorldWarpHole(isInWorld))
        return false;
    if (worldIndex != curWorldId || isDisabled)
        return false;
    if (progressBitflag.countOnBit())
        return progressBitflag.isOnBit(scenarioNo - 1);
    return true;
}

bool GameDataFile::HintInfo::testFunc(s32 curWorldId, bool isGameClear, s32 scenarioNo,
                                      bool isInWorld) const {
    if (isDisableByWorldWarpHole(isInWorld))
        return false;
    if (worldIndex != curWorldId || isDisabled || isGameClear)
        return false;
    if (progressBitflag.countOnBit())
        return progressBitflag.isOnBit(scenarioNo - 1);
    return true;
}

void GameDataFile::unlockAchievementShineName() {
    for (s32 i = 0; i < mHintList.size(); i++)
        if (mHintList[i].achievementStatus == AchievementStatus::None && mHintList[i].isAchievement)
            mHintList[i].achievementStatus = AchievementStatus::Unlocked;
    mIsUnlockAchievement = true;
}

bool GameDataFile::isKidsMode() const {
    return mIsKidsMode;
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

    for (s32 i = 0; i < mGameDataHolder->getWorldList()->getWorldNum(); i++) {
        if (mUnlockedWorldId[i] < 0)
            continue;
        mWorldWarpIndex[mUnlockedWorldId[i]] = i;
        mWorldWarpNum++;
    }
}

s32 GameDataFile::getScenarioNo(s32 worldId) const {
    return mScenarioNo[worldId];
}

s32 GameDataFile::getMainScenarioNo(s32 worldId) const {
    return mMainScenarioNo[worldId];
}

bool GameDataFile::isEmpty() const {
    return mIsPlayDemoOpening;
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
    s32 sum = 0;
    for (s32 i = 0; i < mShopShineNum.size(); i++)
        sum += mShopShineNum[i];
    return sum;
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
    al::StringTmp<128> objId;
    placementId->makeString(&objId);
    al::StringTmp<128> unusedString;
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
        if (!mGotCheckpoint[i].isEmpty())
            count++;
    return count;
}

s32 GameDataFile::calcWorldWarpHoleThroughNum() const {
    s32 count = 0;
    for (s32 i = 0; i < mIsWorldWarpHoleThrough.size(); i++)
        if (mIsWorldWarpHoleThrough[i])
            count++;
    return count;
}

void GameDataFile::wearCostume(const char* name) {
    mCurrentCostumeName.format("%s", name);
}

void GameDataFile::wearCap(const char* name) {
    mCurrentCapName.format("%s", name);
}

void GameDataFile::enableHintById(s32 shineIndex) {
    if (shineIndex >= 0)
        mHintList[shineIndex].isDisabled = false;
}

inline s32 GameDataFile::getCurrentWorldIdNoDevelop() const {
    return sead::Mathi::max(mCurrentWorldId, 0);
}
