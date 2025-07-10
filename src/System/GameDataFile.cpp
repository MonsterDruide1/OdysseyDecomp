#include "System/GameDataFile.h"

#include "Library/Base/StringUtil.h"

#include "System/GameDataFunction.h"
#include "System/GameProgressData.h"

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
    status = 0;
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

bool GameDataFile::HintInfo::isEnableUnlock(s32 world_id_, bool is_moon_rock_, s32 scenario_no,
                                            bool is_game_clear) const {
    if (isDisableByWorldWarpHole(is_game_clear))
        return false;
    if (worldId != world_id_)
        return false;
    if (isDisabled)
        return false;
    if (!(is_moon_rock_ || progressBitFlag.countOnBit() == 0 ||
          progressBitFlag.isOnBit(scenario_no - 1)))
        return false;
    if (isGet)
        return false;
    if (hintStatus != HintStatus_Lock)
        return false;
    if (is_moon_rock_ ? isMoonRock : !isMoonRock)
        return true;
    return false;
}

bool GameDataFile::HintInfo::isHintStatusUnlock(s32 world_id_, s32 scenario_no,
                                                bool is_moon_rock_) const {
    if (worldId != world_id_)
        return false;
    if (isGet)
        return false;
    if (hintStatus == HintStatus_Lock)
        return false;
    if (is_moon_rock_ ? isMoonRock : !isMoonRock) {
        if (GameDataFunction::isCityWorldCeremonyAll(world_id_, scenario_no))
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

bool GameDataFile::HintInfo::isEnableNameUnlockByScenario(s32 world_id_, s32 scenario_no,
                                                          bool is_game_clear) const {
    if (isDisableByWorldWarpHole(is_game_clear))
        return false;
    if (worldId != world_id_)
        return false;
    if (isDisabled)
        return false;
    return progressBitFlag.countOnBit() == 0 || progressBitFlag.isOnBit(scenario_no - 1);
}

bool GameDataFile::isGameClear() const {
    return isUnlockedWorld(GameDataFunction::getWorldIndexPeach()) &&
           isAlreadyGoWorld(GameDataFunction::getWorldIndexPeach());
}

bool GameDataFile::isUnlockedWorld(s32 world_id) const {
    return mGameProgressData->isUnlockWorld(world_id);
}

bool GameDataFile::isAlreadyGoWorld(s32 world_id) const {
    return mGameProgressData->isAlreadyGoWorld(world_id);
}
