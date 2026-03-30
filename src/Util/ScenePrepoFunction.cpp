#include "Util/ScenePrepoFunction.h"

#include <nn/account.h>
#include <nn/prepo.h>
#include <nn/time.h>

#include "Library/Base/HashCodeUtil.h"

#include "System/GameConfigData.h"
#include "System/GameDataFunction.h"
#include "System/GameDataUtil.h"
#include "Util/AchievementUtil.h"
#include "Util/ClothUtil.h"

namespace rs {
static char buffer[0x2000];

bool trySavePrepoCompleteMainScenario(s32 worldId, s32 mainScenarioNo, s64 playTime, s64 saveDataId,
                                      s64 acrossPlayTime) {
    nn::account::Uid lastOpenedUser;
    nn::prepo::PlayReport playReport = nn::prepo::PlayReport("complete_main_scenario_event");

    if (nn::account::GetLastOpenedUser(&lastOpenedUser).IsFailure() || !lastOpenedUser.IsValid())
        return false;

    playReport.SetBuffer(&buffer, nn::prepo::PlayReport::CalcBufferSize(6));

    playReport.Add("world_id", static_cast<s64>(worldId));
    playReport.Add("main_scenario_no", static_cast<s64>(mainScenarioNo));
    playReport.Add("play_time", playTime);
    playReport.Add("across_play_time", acrossPlayTime);
    playReport.Add("save_data_id", saveDataId);
    playReport.Add("app_version", 1l);

    return playReport.Save(lastOpenedUser).IsSuccess();
}

bool trySavePrepoStartStage(const char* stageName, const sead::Vector3f& position, s64 playTime,
                            s64 saveDataId, s64 acrossPlayTime) {
    nn::account::Uid lastOpenedUser;
    nn::prepo::PlayReport playReport = nn::prepo::PlayReport("stage_in_out_event");

    if (nn::account::GetLastOpenedUser(&lastOpenedUser).IsFailure() || !lastOpenedUser.IsValid())
        return false;

    // BUG: buffer too small (prepared for 8 entries, adds 9)
    playReport.SetBuffer(&buffer, nn::prepo::PlayReport::CalcBufferSize(8));

    playReport.Add("stage_name", static_cast<s64>(prepo::calcPrepoHashCode(stageName)));
    playReport.Add("x", position.x);
    playReport.Add("y", position.y);
    playReport.Add("z", position.z);
    playReport.Add("in_out_type", InOutType_Start);
    playReport.Add("play_time", playTime);
    playReport.Add("across_play_time", acrossPlayTime);
    playReport.Add("save_data_id", saveDataId);
    playReport.Add("app_version", 1l);

    return playReport.Save(lastOpenedUser).IsSuccess();
}

bool trySavePrepoExitStage(const char* stageName, const sead::Vector3f& position, s64 playTime,
                           s64 saveDataId, s64 acrossPlayTime) {
    nn::account::Uid lastOpenedUser;
    nn::prepo::PlayReport playReport = nn::prepo::PlayReport("stage_in_out_event");

    if (nn::account::GetLastOpenedUser(&lastOpenedUser).IsFailure() || !lastOpenedUser.IsValid())
        return false;

    // BUG: buffer too small (prepared for 8 entries, adds 9)
    playReport.SetBuffer(&buffer, nn::prepo::PlayReport::CalcBufferSize(8));

    playReport.Add("stage_name", static_cast<s64>(prepo::calcPrepoHashCode(stageName)));
    playReport.Add("x", position.x);
    playReport.Add("y", position.y);
    playReport.Add("z", position.z);
    playReport.Add("in_out_type", InOutType_Exit);
    playReport.Add("play_time", playTime);
    playReport.Add("across_play_time", acrossPlayTime);
    playReport.Add("save_data_id", saveDataId);
    playReport.Add("app_version", 1l);

    return playReport.Save(lastOpenedUser).IsSuccess();
}

bool trySavePrepoMissEvent(const char* stageName, const sead::Vector3f& position, s64 playTime,
                           s64 saveDataId, s64 acrossPlayTime) {
    nn::account::Uid lastOpenedUser;
    nn::prepo::PlayReport playReport = nn::prepo::PlayReport("stage_in_out_event");

    if (nn::account::GetLastOpenedUser(&lastOpenedUser).IsFailure() || !lastOpenedUser.IsValid())
        return false;

    // BUG: buffer too small (prepared for 8 entries, adds 9)
    playReport.SetBuffer(&buffer, nn::prepo::PlayReport::CalcBufferSize(8));

    playReport.Add("stage_name", static_cast<s64>(prepo::calcPrepoHashCode(stageName)));
    playReport.Add("x", position.x);
    playReport.Add("y", position.y);
    playReport.Add("z", position.z);
    playReport.Add("in_out_type", InOutType_Miss);
    playReport.Add("play_time", playTime);
    playReport.Add("across_play_time", acrossPlayTime);
    playReport.Add("save_data_id", saveDataId);
    playReport.Add("app_version", 1l);

    return playReport.Save(lastOpenedUser).IsSuccess();
}

bool trySavePrepoShineGetEvent(const char* stageName, s32 shineId, s32 totalShineNum,
                               s32 totalShopShineNum, s64 playTime, s64 saveDataId,
                               s64 acrossPlayTime) {
    nn::account::Uid lastOpenedUser;
    nn::prepo::PlayReport playReport = nn::prepo::PlayReport("shine_get_event");

    if (nn::account::GetLastOpenedUser(&lastOpenedUser).IsFailure() || !lastOpenedUser.IsValid())
        return false;

    playReport.SetBuffer(&buffer, nn::prepo::PlayReport::CalcBufferSize(8));

    playReport.Add("stage_name", static_cast<s64>(prepo::calcPrepoHashCode(stageName)));
    playReport.Add("shine_id", static_cast<s64>(shineId));
    playReport.Add("total_shine_num", static_cast<s64>(totalShineNum));
    playReport.Add("total_shop_shine_num", static_cast<s64>(totalShopShineNum));
    playReport.Add("play_time", playTime);
    playReport.Add("across_play_time", acrossPlayTime);
    playReport.Add("save_data_id", saveDataId);
    playReport.Add("app_version", 1l);

    return playReport.Save(lastOpenedUser).IsSuccess();
}

bool trySavePrepoReceiveAchievement(const char* achievementName, s32 receivedNum, s64 playTime,
                                    s64 saveDataId, s64 acrossPlayTime) {
    nn::account::Uid lastOpenedUser;
    nn::prepo::PlayReport playReport = nn::prepo::PlayReport("receive_achievement_event");

    if (nn::account::GetLastOpenedUser(&lastOpenedUser).IsFailure() || !lastOpenedUser.IsValid())
        return false;

    playReport.SetBuffer(&buffer, nn::prepo::PlayReport::CalcBufferSize(6));

    playReport.Add("achievement_name", static_cast<s64>(prepo::calcPrepoHashCode(achievementName)));
    playReport.Add("received_num", static_cast<s64>(receivedNum));
    playReport.Add("play_time", playTime);
    playReport.Add("across_play_time", acrossPlayTime);
    playReport.Add("save_data_id", saveDataId);
    playReport.Add("app_version", 1l);

    return playReport.Save(lastOpenedUser).IsSuccess();
}

bool trySavePrepoAchievementProgress(GameDataHolderAccessor accessor, s64 playTime, s64 saveDataId,
                                     s64 acrossPlayTime) {
    nn::account::Uid lastOpenedUser;
    nn::prepo::PlayReport playReport = nn::prepo::PlayReport("achievement_progress_state");

    if (nn::account::GetLastOpenedUser(&lastOpenedUser).IsFailure() || !lastOpenedUser.IsValid())
        return false;

    playReport.SetBuffer(&buffer, nn::prepo::PlayReport::CalcBufferSize(38));

    playReport.Add("shine_gather", static_cast<s64>(GameDataFunction::getTotalShineNum(accessor)));
    playReport.Add("shine_collect_coin_shop",
                   static_cast<s64>(calcBuyItemNumForCoinCollectByWorld(accessor)));
    playReport.Add("shine_shine_2d", static_cast<s64>(calcGetShineNumDot(accessor)));
    playReport.Add("shine_treasure_box", static_cast<s64>(calcGetShineNumTreasureBox(accessor)));
    playReport.Add("shine_music_note", static_cast<s64>(calcGetShineNumNoteObj(accessor)));
    // typo: athretic / athletic
    playReport.Add("shine_timer_athretic",
                   static_cast<s64>(calcGetShineNumTimerAthletic(accessor)));
    playReport.Add("shine_captain_kinopio",
                   static_cast<s64>(calcGetShineNumKinopioBrigade(accessor)));
    playReport.Add("shine_traveling_peach",
                   static_cast<s64>(calcGetShineNumWorldTravelingPeach(accessor)));
    playReport.Add("shine_collect_animal_all",
                   static_cast<s64>(calcGetShineNumCollectAnimal(accessor)));
    playReport.Add("shine_kuribo_girl", static_cast<s64>(calcGetShineNumKuriboGirl(accessor)));
    playReport.Add("shine_jugem", static_cast<s64>(calcGetShineNumJugemFish(accessor)));
    playReport.Add("shine_seed", static_cast<s64>(calcGetShineNumGrowPlant(accessor)));
    playReport.Add("shine_rabbit", static_cast<s64>(calcGetShineNumRabbit(accessor)));
    playReport.Add("shine_dig_point", static_cast<s64>(calcGetShineNumDigPoint(accessor)));
    playReport.Add("shine_cap_hanger", static_cast<s64>(calcGetShineNumCapHanger(accessor)));
    playReport.Add("shine_bird", static_cast<s64>(calcGetShineNumBird(accessor)));
    playReport.Add("shine_costume_room", static_cast<s64>(calcGetShineNumCostumeRoom(accessor)));
    playReport.Add("shine_cap_throttle", static_cast<s64>(calcGetShineNumSlot(accessor)));
    playReport.Add("shine_hide_and_seek_cap_man",
                   static_cast<s64>(calcGetShineNumHideAndSeekCapMan(accessor)));
    playReport.Add("shine_collect_bgm", static_cast<s64>(calcGetShineNumCollectedBgm(accessor)));
    playReport.Add("shine_hint_photo", static_cast<s64>(calcGetShineNumHintPhoto(accessor)));
    playReport.Add("mini_game_race_man", static_cast<s64>(calcGetShineNumRace(accessor)));
    playReport.Add("mini_game_figure_walker",
                   static_cast<s64>(calcGetShineNumFigureWalking(accessor)));
    playReport.Add("mini_game_sphinx_quiz", static_cast<s64>(calcSphinxQuizCompleteNum(accessor)));
    playReport.Add("souvenir_count",
                   static_cast<s64>(calcHaveGiftNum(accessor) + calcHaveStickerNum(accessor)));
    playReport.Add("capture_count", static_cast<s64>(calcHackObjNum(accessor)));
    playReport.Add("costume_cap", static_cast<s64>(calcHaveCapNum(accessor)));
    playReport.Add("costume_clothes", static_cast<s64>(calcHaveClothNum(accessor)));
    playReport.Add("other_moon_stone_all", static_cast<s64>(calcUnlockMoonRockNum(accessor)));
    playReport.Add("other_world_warp_hole_all",
                   static_cast<s64>(calcWorldWarpHoleThroughNum(accessor)));
    playReport.Add("other_check_point", static_cast<s64>(calcGetCheckpointNum(accessor)));
    playReport.Add("other_coin", static_cast<s64>(getTotalCoinNum(accessor)));
    playReport.Add("other_jump", static_cast<s64>(getPlayerJumpCount(accessor)));
    playReport.Add("other_cap_throw", static_cast<s64>(getPlayerThrowCapCount(accessor)));
    playReport.Add("play_time", playTime);
    playReport.Add("across_play_time", acrossPlayTime);
    playReport.Add("save_data_id", saveDataId);
    playReport.Add("app_version", 1l);

    return playReport.Save(lastOpenedUser).IsSuccess();
}

bool trySavePrepoGetCapEvent(const char* itemName, s32 totalNum, s64 playTime, s64 saveDataId,
                             s64 acrossPlayTime) {
    nn::account::Uid lastOpenedUser;
    nn::prepo::PlayReport playReport = nn::prepo::PlayReport("collect_item_get_event");

    if (nn::account::GetLastOpenedUser(&lastOpenedUser).IsFailure() || !lastOpenedUser.IsValid())
        return false;

    playReport.SetBuffer(&buffer, nn::prepo::PlayReport::CalcBufferSize(7));

    playReport.Add("category", ItemCategoryType_Cap);
    playReport.Add("item_name", static_cast<s64>(prepo::calcPrepoHashCode(itemName)));
    playReport.Add("total_num", static_cast<s64>(totalNum));
    playReport.Add("play_time", playTime);
    playReport.Add("across_play_time", acrossPlayTime);
    playReport.Add("save_data_id", saveDataId);
    playReport.Add("app_version", 1l);

    return playReport.Save(lastOpenedUser).IsSuccess();
}

bool trySavePrepoGetClothEvent(const char* itemName, s32 totalNum, s64 playTime, s64 saveDataId,
                               s64 acrossPlayTime) {
    nn::account::Uid lastOpenedUser;
    nn::prepo::PlayReport playReport = nn::prepo::PlayReport("collect_item_get_event");

    if (nn::account::GetLastOpenedUser(&lastOpenedUser).IsFailure() || !lastOpenedUser.IsValid())
        return false;

    playReport.SetBuffer(&buffer, nn::prepo::PlayReport::CalcBufferSize(7));

    playReport.Add("category", ItemCategoryType_Cloth);
    playReport.Add("item_name", static_cast<s64>(prepo::calcPrepoHashCode(itemName)));
    playReport.Add("total_num", static_cast<s64>(totalNum));
    playReport.Add("play_time", playTime);
    playReport.Add("across_play_time", acrossPlayTime);
    playReport.Add("save_data_id", saveDataId);
    playReport.Add("app_version", 1l);

    return playReport.Save(lastOpenedUser).IsSuccess();
}

bool trySavePrepoGetGiftEvent(const char* itemName, s32 totalNum, s64 playTime, s64 saveDataId,
                              s64 acrossPlayTime) {
    nn::account::Uid lastOpenedUser;
    nn::prepo::PlayReport playReport = nn::prepo::PlayReport("collect_item_get_event");

    if (nn::account::GetLastOpenedUser(&lastOpenedUser).IsFailure() || !lastOpenedUser.IsValid())
        return false;

    playReport.SetBuffer(&buffer, nn::prepo::PlayReport::CalcBufferSize(7));

    playReport.Add("category", ItemCategoryType_Gift);
    playReport.Add("item_name", static_cast<s64>(prepo::calcPrepoHashCode(itemName)));
    playReport.Add("total_num", static_cast<s64>(totalNum));
    playReport.Add("play_time", playTime);
    playReport.Add("across_play_time", acrossPlayTime);
    playReport.Add("save_data_id", saveDataId);
    playReport.Add("app_version", 1l);

    return playReport.Save(lastOpenedUser).IsSuccess();
}

bool trySavePrepoGetStickerEvent(const char* itemName, s32 totalNum, s64 playTime, s64 saveDataId,
                                 s64 acrossPlayTime) {
    nn::account::Uid lastOpenedUser;
    nn::prepo::PlayReport playReport = nn::prepo::PlayReport("collect_item_get_event");

    if (nn::account::GetLastOpenedUser(&lastOpenedUser).IsFailure() || !lastOpenedUser.IsValid())
        return false;

    playReport.SetBuffer(&buffer, nn::prepo::PlayReport::CalcBufferSize(7));

    playReport.Add("category", ItemCategoryType_Sticker);
    playReport.Add("item_name", static_cast<s64>(prepo::calcPrepoHashCode(itemName)));
    playReport.Add("total_num", static_cast<s64>(totalNum));
    playReport.Add("play_time", playTime);
    playReport.Add("across_play_time", acrossPlayTime);
    playReport.Add("save_data_id", saveDataId);
    playReport.Add("app_version", 1l);

    return playReport.Save(lastOpenedUser).IsSuccess();
}

bool trySavePrepoChangeCapEvent(const char* itemName, s64 playTime, s64 saveDataId,
                                s64 acrossPlayTime) {
    nn::account::Uid lastOpenedUser;
    nn::prepo::PlayReport playReport = nn::prepo::PlayReport("cloth_change_event");

    if (nn::account::GetLastOpenedUser(&lastOpenedUser).IsFailure() || !lastOpenedUser.IsValid())
        return false;

    playReport.SetBuffer(&buffer, nn::prepo::PlayReport::CalcBufferSize(6));

    playReport.Add("category", ItemCategoryType_Cap);
    playReport.Add("item_name", static_cast<s64>(prepo::calcPrepoHashCode(itemName)));
    playReport.Add("play_time", playTime);
    playReport.Add("across_play_time", acrossPlayTime);
    playReport.Add("save_data_id", saveDataId);
    playReport.Add("app_version", 1l);

    return playReport.Save(lastOpenedUser).IsSuccess();
}

bool trySavePrepoChangeClothEvent(const char* itemName, s64 playTime, s64 saveDataId,
                                  s64 acrossPlayTime) {
    nn::account::Uid lastOpenedUser;
    nn::prepo::PlayReport playReport = nn::prepo::PlayReport("cloth_change_event");

    if (nn::account::GetLastOpenedUser(&lastOpenedUser).IsFailure() || !lastOpenedUser.IsValid())
        return false;

    playReport.SetBuffer(&buffer, nn::prepo::PlayReport::CalcBufferSize(6));

    playReport.Add("category", ItemCategoryType_Cloth);
    playReport.Add("item_name", static_cast<s64>(prepo::calcPrepoHashCode(itemName)));
    playReport.Add("play_time", playTime);
    playReport.Add("across_play_time", acrossPlayTime);
    playReport.Add("save_data_id", saveDataId);
    playReport.Add("app_version", 1l);

    return playReport.Save(lastOpenedUser).IsSuccess();
}

bool trySavePrepoFirstHackEvent(const char* hackObjName, s32 totalHackedObjNum, s64 playTime,
                                s64 saveDataId, s64 acrossPlayTime) {
    nn::account::Uid lastOpenedUser;
    nn::prepo::PlayReport playReport = nn::prepo::PlayReport("first_hack_event");

    if (nn::account::GetLastOpenedUser(&lastOpenedUser).IsFailure() || !lastOpenedUser.IsValid())
        return false;

    playReport.SetBuffer(&buffer, nn::prepo::PlayReport::CalcBufferSize(6));

    playReport.Add("hack_obj_name", static_cast<s64>(prepo::calcPrepoHashCode(hackObjName)));
    playReport.Add("total_hacked_obj_num", static_cast<s64>(totalHackedObjNum));
    playReport.Add("play_time", playTime);
    playReport.Add("across_play_time", acrossPlayTime);
    playReport.Add("save_data_id", saveDataId);
    playReport.Add("app_version", 1l);

    return playReport.Save(lastOpenedUser).IsSuccess();
}

bool trySavePrepoGetCollectBgmEvent(const char* bgmResourceName, const char* bgmSituationName,
                                    bool isBailout, s32 collectedBgmNum, s64 playTime,
                                    s64 saveDataId, s64 acrossPlayTime) {
    nn::account::Uid lastOpenedUser;
    nn::prepo::PlayReport playReport = nn::prepo::PlayReport("get_collect_bgm_event");

    if (nn::account::GetLastOpenedUser(&lastOpenedUser).IsFailure() || !lastOpenedUser.IsValid())
        return false;

    playReport.SetBuffer(&buffer, nn::prepo::PlayReport::CalcBufferSize(8));

    playReport.Add("bgm_resource_name",
                   static_cast<s64>(prepo::calcPrepoHashCode(bgmResourceName)));
    playReport.Add("bgm_situation_name",
                   static_cast<s64>(prepo::calcPrepoHashCode(bgmSituationName)));
    playReport.Add("is_bailout", static_cast<s64>(isBailout));
    playReport.Add("collected_bgm_num", static_cast<s64>(collectedBgmNum));
    playReport.Add("play_time", playTime);
    playReport.Add("across_play_time", acrossPlayTime);
    playReport.Add("save_data_id", saveDataId);
    playReport.Add("app_version", 1l);

    return playReport.Save(lastOpenedUser).IsSuccess();
}

bool trySavePrepoSettingsState(bool isKidsMode, const char* language,
                               const GameConfigData& configData, s64 saveDataId,
                               s64 acrossPlayTime) {
    nn::account::Uid lastOpenedUser;
    nn::prepo::PlayReport playReport = nn::prepo::PlayReport("settings_state");

    if (nn::account::GetLastOpenedUser(&lastOpenedUser).IsFailure() || !lastOpenedUser.IsValid())
        return false;

    // NOTE: buffer too large (prepared for 14 entries, adds 12)
    playReport.SetBuffer(&buffer, nn::prepo::PlayReport::CalcBufferSize(14));

    playReport.Add("is_kids_mode", static_cast<s64>(isKidsMode));
    playReport.Add("language", static_cast<s64>(prepo::calcPrepoHashCode(language)));
    playReport.Add("camera_stick_sensitivity_level",
                   static_cast<s64>(configData.getCameraStickSensitivityLevel()));
    bool isCameraReverseInputH = configData.isCameraReverseInputH();
    bool isCameraReverseInputV = configData.isCameraReverseInputV();
    playReport.Add("camera_reverse_input",
                   static_cast<s64>((isCameraReverseInputH << 1) | isCameraReverseInputV));
    playReport.Add("is_valid_camera_gyro", static_cast<s64>(configData.isValidCameraGyro()));
    playReport.Add("camera_gyro_sensitivity_level",
                   static_cast<s64>(configData.getCameraGyroSensitivityLevel()));
    playReport.Add("is_use_open_list_additional_button",
                   static_cast<s64>(configData.isUseOpenListAdditionalButton()));
    playReport.Add("is_valid_pad_rumble", static_cast<s64>(configData.isValidPadRumble()));
    playReport.Add("pad_rumble_level", static_cast<s64>(configData.getPadRumbleLevel()));
    playReport.Add("across_play_time", acrossPlayTime);
    playReport.Add("save_data_id", saveDataId);
    playReport.Add("app_version", 1l);

    return playReport.Save(lastOpenedUser).IsSuccess();
}

bool trySavePrepoSeparatePlayMode(bool isSeparatePlayMode, s64 playTime, s64 saveDataId,
                                  s64 acrossPlayTime) {
    nn::account::Uid lastOpenedUser;
    nn::prepo::PlayReport playReport = nn::prepo::PlayReport("separate_play_mode_state");

    if (nn::account::GetLastOpenedUser(&lastOpenedUser).IsFailure() || !lastOpenedUser.IsValid())
        return false;

    playReport.SetBuffer(&buffer, nn::prepo::PlayReport::CalcBufferSize(5));

    playReport.Add("is_separate_play_mode", static_cast<s64>(isSeparatePlayMode));
    playReport.Add("play_time", playTime);
    playReport.Add("across_play_time", acrossPlayTime);
    playReport.Add("save_data_id", saveDataId);
    playReport.Add("app_version", 1l);

    return playReport.Save(lastOpenedUser).IsSuccess();
}

namespace prepo {
void enableIsSavePrepo() {}

void disableIsSavePrepo() {}

s64 generateSaveDataId() {
    if (!nn::time::IsInitialized() && nn::time::Initialize().IsFailure())
        return -1l;

    nn::time::PosixTime time;
    if (nn::time::StandardUserSystemClock::GetCurrentTime(&time).IsFailure())
        return -1l;

    return time.time;
}

u32 calcPrepoHashCode(const char* str) {
    if (!str)
        return 0;

    return al::calcHashCode(str);
}
}  // namespace prepo
}  // namespace rs
