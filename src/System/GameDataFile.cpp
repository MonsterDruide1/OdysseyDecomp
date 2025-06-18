#include "System/GameDataFile.h"

#include "Library/Base/StringUtil.h"

#include "System/GameDataFunction.h"
#include "System/GameProgressData.h"

void GameDataFile::HintInfo::clear() {
    stage_name.clear();
    obj_id.clear();
    scenario_name = nullptr;
    trans.set(sead::Vector3f::zero);
    original_trans.set(sead::Vector3f::zero);
    main_scenario_no = -1;
    world_id = -1;
    is_moon_rock = false;
    is_get = false;
    get_time = 0;
    unique_id = -1;
    hint_status = HintStatus_Lock;
    is_achievement = false;
    is_grand = false;
    status = 0;
    is_shop = false;
    hint_idx = 0;
    optional_id.clear();
    progress_bit_flag = 0;
    is_disabled = false;
    is_enable_hint_in_ceremony = false;
}

bool GameDataFile::HintInfo::isDisableByWorldWarpHole(bool is_game_clear) const {
    if (!is_game_clear && al::isEqualString(optional_id.cstr(), "WorldWarpHoleShine"))
        return true;
    return false;
}

bool GameDataFile::HintInfo::isEnableUnlock(s32 world_id_, bool is_moon_rock_, s32 scenario_no,
                                            bool is_game_clear) const {
    if (isDisableByWorldWarpHole(is_game_clear))
        return false;
    if (world_id != world_id_)
        return false;
    if (is_disabled)
        return false;
    if (!(is_moon_rock_ || progress_bit_flag.countOnBit() == 0 ||
          progress_bit_flag.isOnBit(scenario_no - 1)))
        return false;
    if (is_get)
        return false;
    if (hint_status != HintStatus_Lock)
        return false;
    if (is_moon_rock_ ? is_moon_rock : !is_moon_rock)
        return true;
    return false;
}

bool GameDataFile::HintInfo::isHintStatusUnlock(s32 world_id_, s32 scenario_no,
                                                bool is_moon_rock_) const {
    if (world_id != world_id_)
        return false;
    if (is_get)
        return false;
    if (hint_status == HintStatus_Lock)
        return false;
    if (is_moon_rock_ ? is_moon_rock : !is_moon_rock) {
        if (GameDataFunction::isCityWorldCeremonyAll(world_id_, scenario_no))
            return is_enable_hint_in_ceremony;
        return true;
    }
    return false;
}

bool GameDataFile::HintInfo::isHintStatusUnlockByNpc() const {
    return hint_status == HintStatus_UnlockByNpc && !is_moon_rock;
}

bool GameDataFile::HintInfo::isHintStatusUnlockByAmiibo() const {
    return hint_status == HintStatus_UnlockByAmiibo;
}

bool GameDataFile::HintInfo::isEnableNameUnlockByScenario(s32 world_id_, s32 scenario_no,
                                                          bool is_game_clear) const {
    if (isDisableByWorldWarpHole(is_game_clear))
        return false;
    if (world_id != world_id_)
        return false;
    if (is_disabled)
        return false;
    return progress_bit_flag.countOnBit() == 0 || progress_bit_flag.isOnBit(scenario_no - 1);
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
