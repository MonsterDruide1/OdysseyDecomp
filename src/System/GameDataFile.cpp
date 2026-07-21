#include "System/GameDataFile.h"

#include "Library/Placement/PlacementId.h"

#include "System/GameDataFunction.h"
#include "System/GameProgressData.h"
#include "System/SphinxQuizData.h"
#include "System/WorldList.h"
#include "Util/ScenePrepoFunction.h"

void GameDataFile::HintInfo::clear() {
    stageName.clear();
    objId.clear();
    scenarioName = nullptr;
    trans.set(sead::Vector3f::zero);
    originalTrans.set(sead::Vector3f::zero);
    mainScenarioNo = -1;
    worldId = -1;
    isMoonRock = false;
    isGet = false;
    getTime = 0;
    uniqueId = -1;
    hintStatus = HintStatus_Lock;
    isAchievement = false;
    isGrand = false;
    status = AchievementStatus_None;
    isShop = false;
    hintIdx = 0;
    optionalId.clear();
    progressBitFlag = 0;
    isDisabled = false;
    isEnableHintInCeremony = false;
}

bool GameDataFile::HintInfo::isDisableByWorldWarpHole(bool is_game_clear) const {
    if (!is_game_clear && al::isEqualString(optionalId.cstr(), "WorldWarpHoleShine"))
        return true;
    return false;
}

bool GameDataFile::HintInfo::isEnableUnlock(s32 world_id, bool is_moon_rock, s32 scenario_no,
                                            bool is_game_clear) const {
    if (isDisableByWorldWarpHole(is_game_clear))
        return false;
    if (worldId != world_id)
        return false;
    if (isDisabled)
        return false;
    if (!(is_moon_rock || progressBitFlag.countOnBit() == 0 ||
          progressBitFlag.isOnBit(scenario_no - 1)))
        return false;
    if (isGet)
        return false;
    if (hintStatus != HintStatus_Lock)
        return false;
    if (is_moon_rock ? isMoonRock : !isMoonRock)
        return true;
    return false;
}

bool GameDataFile::HintInfo::isHintStatusUnlock(s32 world_id, s32 scenario_no,
                                                bool is_moon_rock) const {
    if (worldId != world_id)
        return false;
    if (isGet)
        return false;
    if (hintStatus == HintStatus_Lock)
        return false;
    if (is_moon_rock ? isMoonRock : !isMoonRock) {
        if (GameDataFunction::isCityWorldCeremonyAll(world_id, scenario_no))
            return isEnableHintInCeremony;
        return true;
    }
    return false;
}

bool GameDataFile::HintInfo::isHintStatusUnlockByNpc() const {
    return hintStatus == HintStatus_UnlockByNpc && !isMoonRock;
}

bool GameDataFile::HintInfo::isHintStatusUnlockByAmiibo() const {
    return hintStatus == HintStatus_UnlockByAmiibo;
}

bool GameDataFile::HintInfo::isEnableNameUnlockByScenario(s32 world_id, s32 scenario_no,
                                                          bool is_game_clear) const {
    if (isDisableByWorldWarpHole(is_game_clear))
        return false;
    if (worldId != world_id)
        return false;
    if (isDisabled)
        return false;
    return progressBitFlag.countOnBit() == 0 || progressBitFlag.isOnBit(scenario_no - 1);
}

void GameDataFile::unlockAchievementShineName() {
    for (s32 i = 0; i < mHintList.size(); i++)
        if (mHintList[i].status == AchievementStatus_None && mHintList[i].isAchievement)
            mHintList[i].status = AchievementStatus_Unlocked;
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

u64 GameDataFile::getLastUpdateTime() const {
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

void GameDataFile::resetTempData() {}

s64 GameDataFile::getPlayTimeTotal() const {
    return mPlayTimeTotal;
}

void GameDataFile::resetSaveDataIdForPrepoForWrite() {
    mSaveDataIdForPrepoForWrite = mSaveDataIdForPrepo;
}

PlayerHitPointData* GameDataFile::getPlayerHitPointData() const {
    return mPlayerHitPointData;
}

bool GameDataFile::isUseMissRestartInfo() const {
    return mIsUseMissRestartInfo;
}

bool GameDataFile::isPayShineAllInAllWorld() const {
    return mIsPayShineAllInAllWorld;
}

s32 GameDataFile::getKeyNum() const {
    return mKeyNum;
}

s32 GameDataFile::getCoinCollectGotNum(s32 world_id) const {
    return mCoinCollectGotNum[world_id];
}

bool GameDataFile::isPayCoinToSphinx() const {
    return mIsPayCoinToSphinx;
}

bool GameDataFile::isStartWorldTravelingPeach() const {
    return mIsStartWorldTravelingPeach;
}

void GameDataFile::startWorldTravelingPeach() {
    mIsStartWorldTravelingPeach = true;
}

s32 GameDataFile::getPlayerJumpCount() const {
    return mPlayerJumpCount;
}

s32 GameDataFile::getPlayerThrowCapCount() const {
    return mPlayerThrowCapCount;
}

s32 GameDataFile::getCheckpointNumMaxInWorld() const {
    return 16;
}

void GameDataFile::startYukimaruRace() {
    mRaceType = RaceType_Yukimaru;
}

void GameDataFile::startYukimaruRaceTutorial() {
    mRaceType = RaceType_YukimaruTutorial;
}

void GameDataFile::startRaceManRace() {
    mRaceType = RaceType_Flag;
}

void GameDataFile::unlockHint() {
    unlockHintImpl(HintStatus_UnlockByNpc);
}

void GameDataFile::unlockHintAmiibo() {
    unlockHintImpl(HintStatus_UnlockByAmiibo);
}

bool GameDataFile::isUnlockAchievementShineName() const {
    return mIsUnlockAchievement;
}

void GameDataFile::winRace() {
    mRaceResult = RaceResult_Win;
}

s32 GameDataFile::getMiniGameNumMax() const {
    return 4;
}

bool GameDataFile::isExistTimeBalloonNpc() const {
    return mIsExistTimeBalloonNpc;
}

const sead::Vector3f& GameDataFile::getTimeBalloonNpcTrans() const {
    return mTimeBalloonNpcTrans;
}

bool GameDataFile::isExistPoetter() const {
    return mIsExistPoetter;
}

const sead::Vector3f& GameDataFile::getPoetterTrans() const {
    return mPoetterTrans;
}

bool GameDataFile::isAlreadyShowExplainCheckpointFlag() const {
    return mIsShowExplainCheckpointFlag;
}

s32 GameDataFile::getShopNpcIconNumMax() const {
    return 4;
}

s32 GameDataFile::getScenarioNoPlacement() const {
    return mScenarioNoPlacement;
}

bool GameDataFile::isFlagOnTalkMessageInfo(s32 index) const {
    return mFlagTalkMessage[index];
}

bool GameDataFile::isTalkKakku() const {
    return mIsTalkKakku;
}

void GameDataFile::talkKakku() {
    mIsTalkKakku = true;
}

bool GameDataFile::isTalkWorldTravelingPeach() const {
    return mIsTalkWorldTravelingPeach;
}

void GameDataFile::talkWorldTravelingPeach() {
    mIsTalkWorldTravelingPeach = true;
}

bool GameDataFile::isTalkCollectBgmNpc() const {
    return mIsTalkCollectBgmNpc;
}

void GameDataFile::talkCollectBgmNpc() {
    mIsTalkCollectBgmNpc = true;
}

s32 GameDataFile::getTokimekiMayorNpcFavorabilityRating() const {
    return mTokimekiMayorNpcFavorabilityRating;
}

void GameDataFile::setTokimekiMayorNpcFavorabilityRating(s32 rating) {
    mTokimekiMayorNpcFavorabilityRating = rating;
}

bool GameDataFile::isFirstNetwork() const {
    return mIsFirstNetwork;
}
