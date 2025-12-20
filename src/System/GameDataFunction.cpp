#include "System/GameDataFunction.h"

#include <g3d/aglNW4FToNN.h>

#include "Library/Area/AreaObj.h"
#include "Library/Area/AreaObjUtil.h"
#include "Library/Base/Macros.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementId.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Resource/ResourceFunction.h"
#include "Library/Scene/Scene.h"
#include "Library/Scene/SceneObjHolder.h"
#include "Library/Scene/SceneObjUtil.h"
#include "Library/Yaml/ByamlUtil.h"

#include "Camera/ScenarioStartCamera.h"
#include "Item/Shine.h"
#include "Item/ShineInfo.h"
#include "Layout/ShopLayoutInfo.h"
#include "MapObj/RiseMapPartsHolder.h"
#include "Npc/WorldTravelingNpc.h"
#include "Player/PlayerFunction.h"
#include "Scene/QuestInfo.h"
#include "Scene/QuestInfoHolder.h"
#include "Scene/TemporaryScenarioCameraHolder.h"
#include "Sequence/ChangeStageInfo.h"
#include "System/GameDataUtil.h"
#include "System/GameProgressData.h"
#include "System/MapDataHolder.h"
#include "System/MoonRockData.h"
#include "System/PlayerHitPointData.h"
#include "System/WorldList.h"
#include "Util/ClothUtil.h"
#include "Util/ScenePrepoFunction.h"
#include "Util/SpecialBuildUtil.h"
#include "Util/StageLayoutFunction.h"

namespace GameDataFunction {

GameDataHolder* getGameDataHolder(const al::IUseSceneObjHolder* scene_obj_holder) {
    return al::getSceneObj<GameDataHolder>(scene_obj_holder);
}

GameDataHolder* getGameDataHolder(const al::SceneObjHolder* scene_obj_holder) {
    return scene_obj_holder->getObj<GameDataHolder>();
}

GameDataHolder* getGameDataHolder(al::GameDataHolderBase* game_data_holder) {
    return static_cast<GameDataHolder*>(game_data_holder);
}

GameDataFile* getGameDataFile(GameDataHolderWriter writer) {
    return writer->getGameDataFile();
}

void setPlayingFileId(GameDataHolderWriter writer, s32 file_id) {
    writer->setPlayingFileId(file_id);
}

void initializeData(GameDataHolderWriter writer, s32 file_id) {
    writer->initializeDataId(file_id);
}

bool isPlayDemoOpening(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isPlayDemoOpening();
}

bool isGameClear(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isGameClear();
}

void setRequireSave(GameDataHolderWriter writer) {
    writer->setRequireSave();
}

void setRequireSaveFrame(GameDataHolderWriter writer) {
    writer->setRequireSaveFrame();
}

s64 getPlayTimeTotal(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getPlayTimeTotal();
}

s64 getPlayTimeAcrossFile(GameDataHolderAccessor accessor) {
    return accessor->getPlayTimeAcrossFile();
}

s64 getSaveDataIdForPrepo(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getSaveDataIdForPrepo();
}

void startDemoStage(GameDataHolderWriter writer, const char* stage_name) {
    writer->getGameDataFile()->startDemoStage(stage_name);
}

bool tryChangeNextStage(GameDataHolderWriter writer, const ChangeStageInfo* info) {
    if (info->isEmpty())
        return false;
    writer->changeNextStage(info);
    return true;
}

bool tryChangeNextStageWithStartRaceFlag(GameDataHolderWriter writer, const ChangeStageInfo* info) {
    if (info->isEmpty())
        return false;
    writer->changeNextStage(info, GameDataFile::RaceType_Flag);
    return true;
}

bool tryChangeNextStageWithStartRaceYukimaru(GameDataHolderWriter writer,
                                             const ChangeStageInfo* info) {
    if (info->isEmpty())
        return false;
    writer->changeNextStage(info, GameDataFile::RaceType_Yukimaru);
    return true;
}

bool tryChangeNextStageWithDemoWorldWarp(GameDataHolderWriter writer, const char* stage_name) {
    writer->changeNextStageWithDemoWorldWarp(stage_name);
    return true;
}

bool tryChangeNextStageWithWorldWarpHole(GameDataHolderWriter writer, const char* stage_name) {
    return writer->tryChangeNextStageWithWorldWarpHole(stage_name);
}

void changeNextStageWithStartTimeBalloon(GameDataHolderWriter writer, s32 scenario_no) {
    ChangeStageInfo info{writer, "TimeBalloonStart", getCurrentStageName(writer),
                         false,  scenario_no,        ChangeStageInfo::RESET_MINI_GAME1};
    writer->changeNextStage(&info);
}

const char* getCurrentStageName(GameDataHolderAccessor accessor) {
    return accessor->getCurrentStageName();
}

void changeNextStageWithEndTimeBalloon(GameDataHolderWriter writer) {
    ChangeStageInfo info{writer, "TimeBalloonStart", getCurrentStageName(writer)};
    writer->changeNextStage(&info);
}

void changeNextStageWithCloset(GameDataHolderWriter writer) {
    ChangeStageInfo info{writer, "ClosetStart", getCurrentStageName(writer)};
    writer->changeNextStage(&info);
}

void findAreaAndChangeNextStage(GameDataHolderWriter writer, const al::LiveActor* actor,
                                const sead::Vector3f* pos_override) {
    if (writer->getGameDataFile()->get_a2c() &&
        writer->getGameDataFile()->getChangeStageInfo()->getSubScenarioType() !=
            ChangeStageInfo::DEFAULT_SUB_SCENARIO)
        return;
    sead::Vector3f pos = al::getTrans(actor);
    if (pos_override)
        pos.set(*pos_override);
    al::AreaObj* area = al::tryFindAreaObj(actor, "ChangeStageArea", pos);
    ChangeStageInfo info{writer, *area->getPlacementInfo()};
    tryChangeNextStage(writer, &info);
}

void returnPrevStage(GameDataHolderWriter writer) {
    writer->returnPrevStage();
}

bool isTimeBalloonSequence(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getChangeStageInfo()->getSubScenarioType() ==
           ChangeStageInfo::RESET_MINI_GAME1;
}

const char* getNextStageName(GameDataHolderAccessor accessor) {
    return accessor->getNextStageName();
}

const char* getCurrentStageName(GameDataHolderAccessor accessor, s32 file_id) {
    return accessor->getCurrentStageName(file_id);
}

static bool isSeaOfTreeStage(const char* stage_name) {
    return al::isEqualString(stage_name, "ForestWorldWoodsStage") ||
           al::isEqualString(stage_name, "ForestWorldWoodsCostumeStage") ||
           al::isEqualString(stage_name, "ForestWorldWoodsTreasureStage");
}

bool isSeaOfTreeStage(GameDataHolderAccessor accessor) {
    return isSeaOfTreeStage(getCurrentStageName(accessor));
}

const char* getNextStageName(GameDataHolderAccessor accessor, s32 file_id) {
    return accessor->getNextStageName(file_id);
}

s32 calcNextScenarioNo(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->calcNextScenarioNo();
}

void restartStage(GameDataHolderWriter writer) {
    writer->getGameDataFile()->restartStage();
}

void missAndRestartStage(GameDataHolderWriter writer) {
    writer->validateCheckpointWarp();
    writer->enableExplainAmiibo();
    writer->getGameDataFile()->missAndRestartStage();
}

bool isMissEndPrevStageForInit(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isMissEndPrevStage();
}

bool isMissEndPrevStageForSceneDead(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isMissEndPrevStage();
}

void reenterStage(GameDataHolderWriter writer) {
    writer->set_4a();
}

s32 getNextWorldId(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getNextWorldId();
}

s32 getPrevWorldId(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getPrevWorldId();
}

s32 getWorldNumForNewReleaseShop(GameDataHolderAccessor accessor) {
    const GameProgressData* game_progress_data = accessor->getGameDataFile()->getGameProgressData();
    s32 unlock_world_num = game_progress_data->getUnlockWorldNum();
    s32 count = 0;
    for (s32 i = 0; i < unlock_world_num; i++) {
        s32 world_id = game_progress_data->getWorldIdForWorldWarpHole(i);
        if (!isAlreadyGoWorld(accessor, world_id))
            continue;
        const char* world_name = getWorldDevelopName(accessor, world_id);
        const sead::PtrArray<sead::FixedSafeString<64>>& list =
            accessor->getWorldsForNewReleaseShop();
        for (s32 i = 0; i < list.size(); i++) {
            if (al::isEqualString(world_name, list[i]->cstr())) {
                count++;
                break;
            }
        }
    }
    return count;
}

// TODO: check what's happening here
static s32 blackBox(s32 value) {
    __asm__("" : "+r"(value));
    return value;
}

template <typename F>
static s32 search(s32 count, F fn) {
    for (s32 i = 0; i < count; i++) {
        s32 status = fn(i);
        if ((blackBox(status | 4) & 7) != 4)
            return status;
    }
    return 2;
}

s32 getWorldIdForNewReleaseShop(GameDataHolderAccessor accessor, s32 index) {
    const GameProgressData* game_progress_data = accessor->getGameDataFile()->getGameProgressData();
    s32 result;
    s32 status = search(game_progress_data->getUnlockWorldNum(), [&](s32 i) {
        s32 world_id = game_progress_data->getWorldIdForWorldWarpHole(i);
        if (!isAlreadyGoWorld(accessor, world_id))
            return 4;
        const char* world_name = getWorldDevelopName(accessor, world_id);
        const sead::PtrArray<sead::FixedSafeString<64>>& list =
            accessor->getWorldsForNewReleaseShop();
        for (s32 i = 0; i < list.size(); i++) {
            if (al::isEqualString(world_name, list[i]->cstr())) {
                if (index == 0) {
                    result = world_id;
                    return 1;
                } else {
                    index--;
                    return 0;
                }
            }
        }
        return 0;
    });
    return status == 2 ? 0 : result;
}

bool isForwardWorldWarpDemo(GameDataHolderAccessor accessor) {
    s32 prev_world_id = getPrevWorldId(accessor);
    s32 next_world_id = getNextWorldId(accessor);
    s32 world_num = getWorldNum(accessor);
    s32 next_world_id_for_demo = -1;
    s32 prev_world_id_for_demo = -1;
    for (s32 i = 0; i < world_num; i++) {
        s32 world_id = accessor->getGameDataFile()->getGameProgressData()->getWorldIdForWorldMap(i);
        if (world_id == prev_world_id)
            prev_world_id_for_demo = i;
        if (world_id == next_world_id)
            next_world_id_for_demo = i;
    }
    if (prev_world_id_for_demo == -1)
        prev_world_id_for_demo = prev_world_id;
    if (next_world_id_for_demo == -1)
        next_world_id_for_demo = next_world_id;
    return prev_world_id_for_demo <= next_world_id_for_demo;
}

s32 getWorldNum(GameDataHolderAccessor accessor) {
    return accessor->getWorldList()->getWorldNum();
}

bool isFirstTimeNextWorld(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isFirstTimeNextWorld();
}

bool checkIsNewWorldInAlreadyGoWorld(GameDataHolderAccessor accessor) {
    s32 world_id = getCurrentWorldIdNoDevelop(accessor);
    if (!isAlreadyGoWorld(accessor, world_id))
        return false;
    const GameProgressData* game_progress_data = accessor->getGameDataFile()->getGameProgressData();
    bool is_after_current_world = false;
    for (s32 i = 0; i < game_progress_data->getUnlockWorldNum(); i++) {
        if (!is_after_current_world) {
            if (game_progress_data->getWorldIdForWorldWarpHole(i) == world_id)
                is_after_current_world = true;
            continue;
        }
        if (accessor->getGameDataFile()->isAlreadyGoWorld(
                game_progress_data->getWorldIdForWorldWarpHole(i)))
            return false;
    }
    return is_after_current_world;
}

static s32 getScenarioNoImpl(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getScenarioNo();
}

s32 getScenarioNo(const al::LiveActor* actor) {
    return getScenarioNoImpl(actor);
}

s32 getScenarioNo(const al::LayoutActor* layout) {
    return getScenarioNoImpl(layout);
}

s32 getScenarioNoPlacement(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getScenarioNoPlacement();
}

bool isEqualScenario(const RiseMapPartsHolder* actor, s32 scenario_no) {
    return getScenarioNo(actor) == scenario_no;
}

s32 getMainQuestMin(const al::LiveActor* actor) {
    GameDataHolderAccessor accessor{actor};
    return accessor->getWorldList()->getMainQuestMin(getCurrentWorldId(actor),
                                                     getScenarioNoImpl(actor));
}

s32 getCurrentWorldId(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getCurrentWorldId();
}

s32 getCurrentWorldIdNoDevelop(GameDataHolderAccessor accessor) {
    return sead::Mathi::clampMin(getCurrentWorldId(accessor), 0);
}

void clearStartId(GameDataHolderWriter writer) {
    writer->getGameDataFile()->clearStartId();
}

void setCheckpointId(GameDataHolderWriter writer, const al::PlacementId* placement_id) {
    writer->setCheckpointId(placement_id);
}

void setRestartPointId(GameDataHolderWriter writer, const al::PlacementId* placement_id) {
    writer->getGameDataFile()->setRestartPointId(placement_id);
}

const char* tryGetRestartPointIdString(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->tryGetRestartPointIdString();
}

const char* tryGetPlayerStartId(GameDataHolderAccessor accessor) {
    return accessor->getNextPlayerStartId();
}

bool isPlayerStartObj(const al::LiveActor* actor, const al::ActorInitInfo& info) {
    return isPlayerStartObj(actor, *info.placementInfo);
}

bool isPlayerStartObj(const al::LiveActor* actor, const al::PlacementInfo& placement_info) {
    const char* str = tryGetPlayerStartId(actor);
    if (!str)
        return false;
    const char* start_id = nullptr;
    bool has_start_id = al::tryGetStringArg(&start_id, placement_info, "ChangeStageId");
    return start_id && has_start_id && al::isEqualString(str, start_id);
}

bool isPlayerStartObj(const al::LiveActor* actor, const char* start_id) {
    const char* str = tryGetPlayerStartId(actor);
    return str && al::isEqualString(str, start_id);
}

bool isPlayerStartLinkedObj(const al::LiveActor* actor, const al::ActorInitInfo& info,
                            const char* link_name) {
    s32 link_num = al::calcLinkChildNum(info, link_name);
    for (s32 i = 0; i < link_num; i++) {
        al::PlacementInfo placement_info;
        al::getLinksInfoByIndex(&placement_info, info, link_name, i);
        if (isPlayerStartObj(actor, placement_info))
            return true;
    }
    return false;
}

void setStartShine(const al::LiveActor* actor, const ShineInfo* info) {
    getGameDataHolder(actor)->getGameDataFile()->setStartShine(info);
}

s32 getStartShineNextIndex(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getStartShineNextIndex();
}

bool isAlreadyShowExplainCheckpointFlag(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isAlreadyShowExplainCheckpointFlag();
}

bool isEnableShowExplainCheckpointFlag(GameDataHolderAccessor accessor) {
    return true;
}

void showExplainCheckpointFlag(GameDataHolderAccessor accessor) {
    accessor->getGameDataFile()->showExplainCheckpointFlag();
}

void getLifeMaxUpItem(const al::LiveActor* actor) {
    if (PlayerFunction::isPlayerDeadStatus(actor))
        return;
    GameDataHolderAccessor accessor{actor};
    accessor->getGameDataFile()->getPlayerHitPointData()->getMaxUpItem();
}

s32 getPlayerHitPoint(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getPlayerHitPointData()->getCurrent();
}

s32 getPlayerHitPointMaxCurrent(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getPlayerHitPointData()->getMaxCurrent();
}

s32 getPlayerHitPointMaxNormal(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getPlayerHitPointData()->getMaxWithoutItem();
}

bool isPlayerHitPointMax(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getPlayerHitPointData()->isMaxCurrent();
}

bool isPlayerHitPointMaxWithItem(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getPlayerHitPointData()->isMaxWithItem();
}

void recoveryPlayer(const al::LiveActor* actor) {
    if (PlayerFunction::isPlayerDeadStatus(actor))
        return;
    GameDataHolderAccessor accessor{actor};
    accessor->getGameDataFile()->getPlayerHitPointData()->recover();
}

void recoveryPlayerForDebug(const al::LiveActor* actor) {
    if (PlayerFunction::isPlayerDeadStatus(actor))
        return;
    GameDataHolderAccessor accessor{actor};
    accessor->getGameDataFile()->getPlayerHitPointData()->recoverForDebug();
}

void recoveryPlayerMax(const al::LiveActor* actor) {
    if (PlayerFunction::isPlayerDeadStatus(actor))
        return;
    GameDataHolderAccessor accessor{actor};
    accessor->getGameDataFile()->getPlayerHitPointData()->recoverMax();
}

void recoveryPlayerForSystem(const GameDataHolder* game_data_holder) {
    game_data_holder->getGameDataFile()->getPlayerHitPointData()->recover();
}

void recoveryPlayerMaxForSystem(const GameDataHolder* game_data_holder) {
    game_data_holder->getGameDataFile()->getPlayerHitPointData()->recoverMax();
}

void initPlayerHitPointForSystem(const GameDataHolder* game_data_holder) {
    game_data_holder->getGameDataFile()->getPlayerHitPointData()->init();
}

void damagePlayer(GameDataHolderWriter writer) {
    writer->getGameDataFile()->getPlayerHitPointData()->damage();
}

void killPlayer(GameDataHolderWriter writer) {
    writer->getGameDataFile()->getPlayerHitPointData()->kill();
}

bool isPlayerLifeZero(GameDataHolderAccessor accessor) {
    return getPlayerHitPoint(accessor) <= 0;
}

bool isGotShine(GameDataHolderAccessor accessor, const ShineInfo* info) {
    return accessor->getGameDataFile()->isGotShine(info);
}

bool isGotShine(GameDataHolderAccessor accessor, const al::PlacementId* placement_id) {
    ShineInfo shine_info;
    shine_info.init(getCurrentStageName(accessor), al::makeStringPlacementId(placement_id).cstr(),
                    nullptr, nullptr, ShineInfo::Type_Shine);
    return isGotShine(accessor, &shine_info);
}

bool isGotShine(GameDataHolderAccessor accessor, const char* stage_name, const char* obj_id) {
    ShineInfo shine_info;
    shine_info.init(stage_name, obj_id, nullptr, nullptr, ShineInfo::Type_Shine);
    return isGotShine(accessor, &shine_info);
}

bool isGotShine(GameDataHolderAccessor accessor, s32 index) {
    return accessor->getGameDataFile()->isGotShine(index);
}

bool isGotShine(const Shine* shine) {
    return shine->isGot();
}

void setGotShine(GameDataHolderWriter writer, const ShineInfo* info) {
    writer->getGameDataFile()->setGotShine(info);
}

s32 getGotShineNum(GameDataHolderAccessor accessor, s32 file_id) {
    return file_id == -1 ? accessor->getGameDataFile()->getShineNum() :
                           accessor->getGameDataFile(file_id)->getShineNum();
}

const ShineInfo* getLatestGetShineInfo(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getLatestGetShineInfo();
}

s32 getCurrentShineNum(GameDataHolderAccessor accessor) {
    if (isGameClear(accessor)) {
        s32 world_num = getWorldNum(accessor);
        s32 count = 0;
        for (s32 i = 0; i < world_num; i++)
            count += sead::Mathi::clampMin(accessor->getGameDataFile()->getShineNum(i) -
                                               accessor->getGameDataFile()->getPayShineNum(i),
                                           0);
        return count;
    }
    return sead::Mathi::clampMin(accessor->getGameDataFile()->getShineNum() -
                                     accessor->getGameDataFile()->getPayShineNum(),
                                 0);
}

s32 getTotalShineNum(GameDataHolderAccessor accessor, s32 file_id) {
    return file_id == -1 ? accessor->getGameDataFile()->getTotalShineNum() :
                           accessor->getGameDataFile(file_id)->getTotalShineNum();
}

s32 getTotalShopShineNum(GameDataHolderAccessor accessor, s32 file_id) {
    return file_id == -1 ? accessor->getGameDataFile()->getTotalShopShineNum() :
                           accessor->getGameDataFile(file_id)->getTotalShopShineNum();
}

bool tryGetNextMainScenarioLabel(sead::BufferedSafeString* out_label,
                                 sead::BufferedSafeString* out_stage_name,
                                 const al::IUseSceneObjHolder* scene_obj_holder) {
    GameDataHolderAccessor accessor{scene_obj_holder};
    return accessor->getGameDataFile()->tryGetNextMainScenarioLabel(out_label, out_stage_name);
}

s32 getMainScenarioNumMax(GameDataHolderAccessor accessor) {
    return 10;
}

void setMainScenarioNo(GameDataHolderWriter writer, s32 scenario_no) {
    writer->getGameDataFile()->setMainScenarioNo(scenario_no);
}

bool tryGetNextMainScenarioPos(sead::Vector3f* out, GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->tryGetNextMainScenarioPos(out);
}

bool isPlayScenarioCamera(GameDataHolderAccessor accessor, const QuestInfo* info) {
    return accessor->getGameDataFile()->isPlayScenarioCamera(info);
}

bool isPlayAlreadyScenarioStartCamera(GameDataHolderAccessor accessor, s32 quest_no) {
    return accessor->isPlayAlreadyScenarioStartCamera(quest_no);
}

bool isEnterStageFirst(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isEnterStageFirst();
}

bool isNextMainShine(GameDataHolderAccessor accessor, s32 index) {
    return accessor->getGameDataFile()->isNextMainShine(index);
}

bool isMainShine(GameDataHolderAccessor accessor, s32 index) {
    return accessor->getGameDataFile()->isMainShine(index);
}

bool isLatestGetMainShine(GameDataHolderAccessor accessor, const ShineInfo* info) {
    return accessor->getGameDataFile()->isLatestGetMainShine(info);
}

s32 tryFindLinkedShineIndex(const al::LiveActor* actor, const al::ActorInitInfo& info) {
    GameDataHolderAccessor accessor{actor};
    return accessor->getGameDataFile()->tryFindLinkedShineIndex(info, actor);
}

s32 tryFindLinkedShineIndex(const al::LiveActor* actor, const al::ActorInitInfo& info,
                            s32 link_index) {
    GameDataHolderAccessor accessor{actor};
    return accessor->getGameDataFile()->tryFindLinkedShineIndex(info, link_index, actor);
}

s32 tryFindLinkedShineIndexByLinkName(const al::LiveActor* actor, const al::ActorInitInfo& info,
                                      const char* link_name) {
    GameDataHolderAccessor accessor{actor};
    return accessor->getGameDataFile()->tryFindLinkedShineIndexByLinkName(actor, info, link_name);
}

s32 calcLinkedShineNum(const al::LiveActor* actor, const al::ActorInitInfo& info) {
    GameDataHolderAccessor accessor{actor};
    return accessor->getGameDataFile()->calcLinkedShineNum(info);
}

s32 tryFindShineIndex(const al::LiveActor* actor, const al::ActorInitInfo& info) {
    GameDataHolderAccessor accessor{actor};
    return accessor->getGameDataFile()->tryFindShineIndex(info);
}

s32 tryFindShineIndex(const al::LiveActor* actor, const char* stage_name, const char* obj_id) {
    GameDataHolderAccessor accessor{actor};
    return accessor->getGameDataFile()->tryFindShineIndex(stage_name, obj_id);
}

s32 tryFindShineUniqueId(const al::Scene* scene, const ShineInfo* shine_info) {
    GameDataHolderAccessor accessor{scene};
    return accessor->getGameDataFile()->tryFindUniqueId(shine_info);
}

void disableHintByShineIndex(const al::LiveActor* actor, s32 index) {
    GameDataHolderAccessor accessor{actor};
    accessor->getGameDataFile()->disableHintById(index);
}

void enableHintByShineIndex(const al::LiveActor* actor, s32 index) {
    GameDataHolderAccessor accessor{actor};
    accessor->getGameDataFile()->enableHintById(index);
}

bool calcIsGetMainShineAll(const al::IUseSceneObjHolder* scene_obj_holder) {
    GameDataHolderAccessor accessor{scene_obj_holder};
    return accessor->getGameDataFile()->calcIsGetMainShineAll(scene_obj_holder);
}

bool calcIsGetShineAllInWorld(GameDataHolderAccessor accessor, s32 world_id) {
    return accessor->getGameDataFile()->calcIsGetShineAllInWorld(world_id);
}

bool calcIsGetShineAllInAllWorld(GameDataHolderAccessor accessor) {
    for (s32 i = 0; i < getWorldNum(accessor); i++)
        if (!calcIsGetShineAllInWorld(accessor, i))
            return false;
    return true;
}

bool tryFindAndInitShineInfoByOptionalId(ShineInfo* info, GameDataHolderAccessor accessor,
                                         const char* optional_id) {
    return accessor->getGameDataFile()->tryFindAndInitShineInfoByOptionalId(info, optional_id);
}

namespace {

class SceneObjAccessor : public al::IUseSceneObjHolder {
public:
    SceneObjAccessor(al::SceneObjHolder* scene_obj_holder) : mSceneObjHolder(scene_obj_holder) {}

    al::SceneObjHolder* getSceneObjHolder() const override { return mSceneObjHolder; }

private:
    al::SceneObjHolder* mSceneObjHolder;
};

}  // namespace

bool isGotLinkedShineBeforeInitActor(const al::ActorInitInfo& actor_info, const char* link_name) {
    SceneObjAccessor scene_obj_accessor{actor_info.actorSceneInfo.sceneObjHolder};
    GameDataHolderAccessor accessor{&scene_obj_accessor};
    return accessor->getGameDataFile()->isGotShine(
        accessor->getGameDataFile()->tryFindLinkedShineIndexByLinkName(&scene_obj_accessor,
                                                                       actor_info, link_name));
}

bool checkIsComplete(const al::IUseSceneObjHolder* scene_obj_holder, s32 file_id) {
    const GameDataFile* game_data_file =
        getGameDataHolder(scene_obj_holder)->getGameDataFile(file_id);
    for (s32 i = 0; i < getWorldNum(scene_obj_holder); i++)
        if (!game_data_file->calcIsGetShineAllInWorld(i))
            return false;

    al::Resource* resource = al::findOrCreateResource("SystemData/HackObjList", nullptr);
    al::ByamlIter iter{al::findResourceYaml(resource, "HackObjList", nullptr)};
    s32 hack_obj_count = iter.getSize();
    for (s32 i = 0; i < hack_obj_count; i++) {
        al::ByamlIter subiter;
        iter.tryGetIterByIndex(&subiter, i);
        if (al::tryGetByamlKeyBoolOrFalse(subiter, "IsDebug"))
            continue;
        const char* hack_name = al::getByamlKeyString(subiter, "HackName");
        if (!isExistInHackDictionary(scene_obj_holder, hack_name, file_id))
            return false;
    }

    if (!rs::isCompleteCloth(scene_obj_holder, file_id) ||
        !rs::isCompleteCap(scene_obj_holder, file_id) ||
        !rs::checkCompleteGift(scene_obj_holder, file_id) ||
        !rs::checkCompleteSticker(scene_obj_holder, file_id))
        return false;

    if (getCollectedBgmNum(scene_obj_holder, file_id) != getCollectedBgmMaxNum(scene_obj_holder))
        return false;

    return true;
}

bool isEnableOpenMoonRock(const al::LiveActor* actor) {
    GameDataHolderAccessor accessor{actor};
    return accessor->getGameDataFile()->isEnableOpenMoonRock(getCurrentWorldIdNoDevelop(actor));
}

void openMoonRock(const al::LiveActor* actor, const al::PlacementId*) {
    GameDataHolderAccessor accessor{actor};
    accessor->getGameDataFile()->unlockHintAddByMoonRock();
}

bool isOpenMoonRock(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isOpenMoonRock(getCurrentWorldIdNoDevelop(accessor));
}

bool isEnableShowDemoOpenMoonRockFirst(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getMoonRockData()->isEnableShowDemoOpenMoonRockFirst();
}

bool isEnableShowDemoOpenMoonRockWorld(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getMoonRockData()->isEnableShowDemoOpenMoonRockWorld(
        getCurrentWorldIdNoDevelop(accessor));
}

void showDemoOpenMoonRockFirst(GameDataHolderAccessor accessor) {
    accessor->getGameDataFile()->getMoonRockData()->showDemoOpenMoonRockFirst();
}

void showDemoOpenMoonRockWorld(GameDataHolderAccessor accessor) {
    accessor->getGameDataFile()->getMoonRockData()->showDemoOpenMoonRockWorld(
        getCurrentWorldIdNoDevelop(accessor));
}

s32 calcMoonRockTalkMessageIndex(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getMoonRockData()->calcMoonRockTalkMessageIndex();
}

void addMoonRockTalkMessageIndex(GameDataHolderWriter writer) {
    writer->getGameDataFile()->getMoonRockData()->addMoonRockTalkMessageIndex(
        getCurrentWorldId(writer));
}

bool isAppearedMoonRockTalkMessage(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getMoonRockData()->isAppearedMoonRockTalkMessage(
        getCurrentWorldId(accessor));
}

void addPayShine(GameDataHolderWriter writer, s32 count) {
    writer->getGameDataFile()->addPayShine(count);
}

void addPayShineCurrentAll(GameDataHolderWriter writer) {
    writer->getGameDataFile()->addPayShineCurrentAll();
}

s32 getPayShineNum(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getPayShineNum();
}

s32 getPayShineNum(GameDataHolderAccessor accessor, s32 world_id) {
    return accessor->getGameDataFile()->getPayShineNum(world_id);
}

s32 getTotalPayShineNum(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getTotalPayShineNum();
}

s32 getTotalPayShineNumClamp(GameDataHolderAccessor accessor) {
    return sead::Mathi::clampMax(getTotalPayShineNum(accessor), 999);
}

bool isPayShineAllInAllWorld(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isPayShineAllInAllWorld();
}

void addKey(GameDataHolderWriter writer, s32 count) {
    writer->getGameDataFile()->addKey(count);
}

s32 getKeyNum(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getKeyNum();
}

s32 getCurrentKeyNum(GameDataHolderAccessor accessor) {
    return getKeyNum(accessor) - getOpenDoorLockNum(accessor);
}

s32 getOpenDoorLockNum(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getOpenDoorLockNum();
}

void openDoorLock(GameDataHolderWriter writer, const al::PlacementId* placement_id) {
    if (isOpenDoorLock(writer, placement_id))
        return;
    writer->getGameDataFile()->addOpenDoorLockNum(1);
    writer->getGameDataFile()->setStartedObj(placement_id);
}

bool isOpenDoorLock(GameDataHolderAccessor accessor, const al::PlacementId* placement_id) {
    return accessor->getGameDataFile()->isStartedObj(placement_id);
}

void setObjStarted(GameDataHolder* game_data_holder, const al::PlacementId* placement_id) {
    game_data_holder->getGameDataFile()->setStartedObj(placement_id);
}

bool isObjStarted(const GameDataHolder* game_data_holder, const al::PlacementId* placement_id) {
    return game_data_holder->getGameDataFile()->isStartedObj(placement_id);
}

bool isObjStarted(GameDataHolderAccessor accessor, const char* stage_name, const char* obj_id) {
    return accessor->getGameDataFile()->isStartedObj(stage_name, obj_id);
}

void saveObjS32(GameDataHolderWriter writer, const al::PlacementId* placement_id, s32 value) {
    writer->getGameDataFile()->setSaveObjS32(placement_id, value);
}

bool tryFindSaveObjS32Value(s32* out, GameDataHolderAccessor accessor,
                            const al::PlacementId* placement_id) {
    if (const GameDataFile::SaveObjS32* obj =
            accessor->getGameDataFile()->tryFindSaveObjS32(placement_id)) {
        *out = obj->value;
        return true;
    }
    return false;
}

void onObjNoWriteSaveData(GameDataHolderWriter writer, const al::PlacementId* placement_id) {
    writer->onObjNoWriteSaveData(placement_id);
}

void offObjNoWriteSaveData(GameDataHolderWriter writer, const al::PlacementId* placement_id) {
    writer->offObjNoWriteSaveData(placement_id);
}

bool isOnObjNoWriteSaveData(GameDataHolderAccessor accessor, const al::PlacementId* placement_id) {
    return accessor->isOnObjNoWriteSaveData(placement_id);
}

void onObjNoWriteSaveDataResetMiniGame(GameDataHolderWriter writer,
                                       const al::PlacementId* placement_id) {
    writer->onObjNoWriteSaveDataResetMiniGame(placement_id);
}

void offObjNoWriteSaveDataResetMiniGame(GameDataHolderWriter writer,
                                        const al::PlacementId* placement_id) {
    writer->offObjNoWriteSaveDataResetMiniGame(placement_id);
}

bool isOnObjNoWriteSaveDataResetMiniGame(GameDataHolderAccessor accessor,
                                         const al::PlacementId* placement_id) {
    return accessor->isOnObjNoWriteSaveDataResetMiniGame(placement_id);
}

void onObjNoWriteSaveDataInSameScenario(GameDataHolder* game_data_holder,
                                        const al::PlacementId* placement_id) {
    game_data_holder->onObjNoWriteSaveDataInSameScenario(placement_id);
}

bool isOnObjNoWriteSaveDataInSameScenario(const GameDataHolder* game_data_holder,
                                          const al::PlacementId* placement_id) {
    return game_data_holder->isOnObjNoWriteSaveDataInSameScenario(placement_id);
}

void setSessionEventProgress(GameDataHolderWriter writer, const SessionEventProgress& progress) {
    writer->getGameDataFile()->setSessionEventProgress(progress);
}

const SessionEventProgress& getSessionEventProgress(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getSessionEventProgress();
}

bool isPayCoinToSphinx(const al::LiveActor* actor) {
    GameDataHolderAccessor accessor{actor};
    return accessor->getGameDataFile()->isPayCoinToSphinx();
}

void payCoinToSphinx(const al::LiveActor* actor) {
    GameDataHolderAccessor accessor{actor};
    accessor->getGameDataFile()->payCoinToSphinx();
}

static GameDataFile::CapStatus getCapStatusForJango(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getCapStatusForJango();
}

static void setCapStatusForJango(GameDataHolderAccessor accessor, GameDataFile::CapStatus status) {
    return accessor->getGameDataFile()->setCapStatusForJango(status);
}

static GameDataFile::CapStatus getCapStatusForJangoSubArea(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getCapStatusForJangoSubArea();
}

static void setCapStatusForJangoSubArea(GameDataHolderAccessor accessor,
                                        GameDataFile::CapStatus status) {
    return accessor->getGameDataFile()->setCapStatusForJangoSubArea(status);
}

bool isRemovedCapByJango(const al::LiveActor* actor) {
    if (!isMainStage(actor))
        return getCapStatusForJangoSubArea(actor) == GameDataFile::CapStatus::Removed;
    return getCapStatusForJango(actor) == GameDataFile::CapStatus::Removed;
}

bool isGetCapFromJango(const al::LiveActor* actor) {
    if (!isMainStage(actor))
        return getCapStatusForJangoSubArea(actor) == GameDataFile::CapStatus::GotBack;
    return getCapStatusForJango(actor) == GameDataFile::CapStatus::GotBack;
}

void removeCapByJango(const al::LiveActor* actor) {
    if (!isMainStage(actor)) {
        setCapStatusForJangoSubArea(actor, GameDataFile::CapStatus::Removed);
        return;
    }
    setCapStatusForJango(actor, GameDataFile::CapStatus::Removed);
}

void getCapFromJango(const al::LiveActor* actor) {
    if (!isMainStage(actor)) {
        setCapStatusForJangoSubArea(actor, GameDataFile::CapStatus::GotBack);
        return;
    }
    setCapStatusForJango(actor, GameDataFile::CapStatus::GotBack);
}

void addJangoCount(const al::LiveActor* actor) {
    GameDataHolderAccessor accessor{actor};
    accessor->getGameDataFile()->addJangoCount();
}

s32 getJangoCount(const al::LiveActor* actor) {
    GameDataHolderAccessor accessor{actor};
    return accessor->getGameDataFile()->getJangoCount();
}

bool isFirstWorldTravelingStatus(const WorldTravelingNpc* actor) {
    GameDataHolderAccessor accessor{actor};
    return accessor->getGameDataFile()->isFirstWorldTravelingStatus();
}

void saveWorldTravelingStatus(const WorldTravelingNpc* actor, const char* status) {
    GameDataHolderAccessor accessor{actor};
    accessor->getGameDataFile()->saveWorldTravelingStatus(status);
}

const char* getWorldTravelingStatus(const WorldTravelingNpc* actor) {
    GameDataHolderAccessor accessor{actor};
    return accessor->getGameDataFile()->getWorldTravelingStatus();
}

bool isRaceStart(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isRaceStart();
}

bool isRaceStartFlag(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getRaceType() == GameDataFile::RaceType_Flag;
}

bool isRaceStartYukimaru(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getRaceType() == GameDataFile::RaceType_Yukimaru;
}

bool isRaceStartYukimaruTutorial(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getRaceType() == GameDataFile::RaceType_YukimaruTutorial;
}

bool isRaceWin(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getRaceResult() == GameDataFile::RaceResult_Win;
}

bool isRaceWin(GameDataHolderAccessor, s32) {
    return false;
}

bool isRaceResultSecond(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getRaceResult() == GameDataFile::RaceResult_Second;
}

bool isRaceResultThird(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getRaceResult() == GameDataFile::RaceResult_Third;
}

bool isRaceLose(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getRaceResult() == GameDataFile::RaceResult_Lose;
}

bool isRaceCancel(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getRaceResult() == GameDataFile::RaceResult_Cancel;
}

void winRace(GameDataHolderWriter writer) {
    writer->getGameDataFile()->winRace();
}

void endRaceResultSecond(GameDataHolderWriter writer) {
    writer->getGameDataFile()->setRaceResult(GameDataFile::RaceResult_Second);
}

void endRaceResultThird(GameDataHolderWriter writer) {
    writer->getGameDataFile()->setRaceResult(GameDataFile::RaceResult_Third);
}

void loseRace(GameDataHolderWriter writer) {
    writer->getGameDataFile()->setRaceResult(GameDataFile::RaceResult_Lose);
}

static s32 getRaceRivalLevelImpl(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getRaceRivalLevel();
}

s32 getRaceRivalLevel(const al::LiveActor* actor) {
    return getRaceRivalLevelImpl(actor);
}

s32 getRaceRivalLevel(const al::Scene* scene) {
    return getRaceRivalLevelImpl(scene);
}

void setRaceRivalLevel(const al::LiveActor* actor, s32 level) {
    GameDataHolderAccessor accessor{actor};
    accessor->getGameDataFile()->setRaceRivalLevel(level);
}

void setLastRaceRanking(GameDataHolderAccessor accessor, s32 ranking) {
    accessor->getGameDataFile()->setLastRaceRanking(ranking);
}

s32 getLastRaceRanking(GameDataHolderWriter writer) {
    return writer->getGameDataFile()->getLastRaceRanking();
}

void incrementRaceLoseCount(const al::Scene* scene, s32 level) {
    GameDataHolderAccessor accessor{scene};
    accessor->getGameDataFile()->incrementRaceLoseCount(level);
}

s32 getRaceLoseCount(const al::LiveActor* actor, s32 level) {
    GameDataHolderAccessor accessor{actor};
    return accessor->getGameDataFile()->getRaceLoseCount(level);
}

void addCoinCollect(GameDataHolderWriter writer, const al::PlacementId* placement_id) {
    writer->getGameDataFile()->addCoinCollect(placement_id);
}

void useCoinCollect(GameDataHolderWriter writer, s32 count) {
    writer->getGameDataFile()->useCoinCollect(count);
}

bool isGotCoinCollect(GameDataHolderAccessor accessor, const al::ActorInitInfo& actor_info) {
    al::PlacementId placement_id;
    al::tryGetPlacementId(&placement_id, actor_info);
    return accessor->getGameDataFile()->isGotCoinCollect(&placement_id);
}

s32 getCoinCollectNum(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getCoinCollectNum();
}

s32 getCoinCollectGotNum(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getCoinCollectGotNum();
}

s32 getCoinCollectGotNum(GameDataHolderAccessor accessor, s32 world_id) {
    return accessor->getGameDataFile()->getCoinCollectGotNum(world_id);
}

s32 getCoinCollectNumMax(GameDataHolderAccessor accessor) {
    return accessor->getCoinCollectNumMax(getCurrentWorldId(accessor));
}

s32 getCoinCollectNumMax(GameDataHolderAccessor accessor, s32 world_id) {
    return accessor->getCoinCollectNumMax(world_id);
}

bool tryFindExistCoinCollectStagePosExcludeHomeStageInCurrentWorld(
    sead::Vector3f* out_pos, const char** out_stage_name, GameDataHolderAccessor accessor) {
    s32 world_id = getCurrentWorldIdNoDevelop(accessor);
    if (const char* stage_name =
            accessor->getGameDataFile()->tryFindExistCoinCollectStageName(world_id)) {
        if (accessor->getWorldList()->tryFindTransOnMainStageByStageName(
                out_pos, stage_name, getWorldScenarioNo(accessor, world_id))) {
            *out_stage_name = stage_name;
            return true;
        }
    }
    return false;
}

s32 getWorldScenarioNo(GameDataHolderAccessor accessor, s32 world_id) {
    return accessor->getGameDataFile()->getScenarioNo(world_id);
}

void addCoin(GameDataHolderWriter writer, s32 count) {
    writer->getGameDataFile()->addCoin(count);
}

void subCoin(GameDataHolderWriter writer, s32 count) {
    writer->getGameDataFile()->addCoin(-count);
}

s32 getCoinNum(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getCoinNum();
}

s32 getTotalCoinNum(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getTotalCoinNum();
}

bool isAppearCourseName(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isAppearCourseName();
}

void setStageHakoniwa(GameDataHolderWriter writer) {
    writer->getGameDataFile()->setStageHakoniwa();
}

bool isEnableCheckpointWarp(GameDataHolderAccessor accessor) {
    return accessor->isEnableCheckpointWarp();
}

void validateCheckpointWarp(GameDataHolderWriter writer) {
    writer->validateCheckpointWarp();
}

void invalidateCheckpointWarp(GameDataHolderWriter writer) {
    writer->invalidateCheckpointWarp();
}

s32 getCheckpointNumMaxInWorld(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getCheckpointNumMaxInWorld();
}

const sead::Vector3f& getCheckpointTransInWorld(GameDataHolderAccessor accessor, s32 index) {
    return accessor->getGameDataFile()->getCheckpointTransInWorld(
        getCheckpointObjIdInWorld(accessor, index));
}

const sead::Vector3f& getCheckpointTransInWorld(GameDataHolderAccessor accessor,
                                                const char* obj_id) {
    return accessor->getGameDataFile()->getCheckpointTransInWorld(obj_id);
}

const char* getCheckpointObjIdInWorld(GameDataHolderAccessor accessor, s32 index) {
    return accessor->getGameDataFile()->getCheckpointObjIdInWorld(index);
}

bool isGotCheckpointInWorld(GameDataHolderAccessor accessor, s32 index) {
    return accessor->getGameDataFile()->isGotCheckpointInWorld(index);
}

bool isGotCheckpoint(GameDataHolderAccessor accessor, al::PlacementId* placement_id) {
    return accessor->getGameDataFile()->isGotCheckpoint(placement_id);
}

s32 calcGotCheckpointNumInWorld(GameDataHolderAccessor accessor) {
    s32 max = getCheckpointNumMaxInWorld(accessor);
    s32 count = 0;
    for (s32 i = 0; i < max; i++)
        if (isGotCheckpointInWorld(accessor, i))
            count++;
    return count;
}

void changeNextSceneByGotCheckpoint(GameDataHolderWriter writer, s32 index) {
    writer->getGameDataFile()->changeNextSceneByGotCheckpoint(index);
}

void changeNextSceneByHome(GameDataHolderWriter writer) {
    writer->getGameDataFile()->changeNextSceneByHome();
}

bool isWarpCheckpoint(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isWarpCheckpoint();
}

const char* getCheckpointWarpObjId(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getCheckpointWarpObjId();
}

void registerCheckpointTrans(GameDataHolderWriter writer, const al::PlacementId* placement_id,
                             const sead::Vector3f& trans) {
    writer->getGameDataFile()->registerCheckpointTrans(placement_id, trans);
}

bool isEnableUnlockHint(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isEnableUnlockHint();
}

void unlockHint(GameDataHolderWriter writer) {
    writer->getGameDataFile()->unlockHint();
}

void unlockHintAmiibo(GameDataHolderWriter writer) {
    writer->getGameDataFile()->unlockHintAmiibo();
}

s32 calcHintNum(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->calcHintNum();
}

s32 calcRestHintNum(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->calcRestHintNum();
}

bool checkExistHint(GameDataHolderAccessor accessor) {
    return calcHintNum(accessor) > 0;
}

s32 getHintNumMax(GameDataHolderAccessor accessor) {
    return 70;
}

const sead::Vector3f& calcHintTrans(GameDataHolderAccessor accessor, s32 index) {
    return accessor->getGameDataFile()->calcHintTrans(index);
}

const sead::Vector3f& getLatestHintTrans(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getLatestHint()->trans;
}

bool checkLatestHintSeaOfTree(GameDataHolderAccessor accessor) {
    return isSeaOfTreeStage(accessor->getGameDataFile()->getLatestHint()->stageName.cstr());
}

s32 calcHintMoonRockNum(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->calcHintMoonRockNum();
}

s32 getHintMoonRockNumMax(GameDataHolderAccessor accessor) {
    return 25;
}

const sead::Vector3f& calcHintMoonRockTrans(GameDataHolderAccessor accessor, s32 index) {
    return accessor->getGameDataFile()->calcHintMoonRockTrans(index);
}

void initializeHintList(GameDataHolderWriter writer) {
    writer->getGameDataFile()->initializeHintList();
}

const sead::Vector3f& calcHintTransMostEasy(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->calcHintTransMostEasy();
}

bool calcHintTransMostNear(sead::Vector3f* out, GameDataHolderAccessor accessor,
                           const sead::Vector3f& pos) {
    s32 hint_num = calcHintNum(accessor);
    if (hint_num == 0)
        return false;
    f32 min_dist = sead::Mathf::maxNumber();
    for (s32 i = 0; i < hint_num; i++) {
        sead::Vector3f trans = calcHintTrans(accessor, i);
        f32 dist = (trans - pos).length();
        if (dist < min_dist) {
            out->set(trans);
            min_dist = dist;
        }
    }
    return true;
}

bool checkHintSeaOfTree(GameDataHolderAccessor accessor, s32 index) {
    return isSeaOfTreeStage(accessor->getGameDataFile()->findHint(index).stageName.cstr());
}

bool checkHintSeaOfTreeMoonRock(GameDataHolderAccessor accessor, s32 index) {
    return isSeaOfTreeStage(accessor->getGameDataFile()->findHintMoonRock(index).stageName.cstr());
}

s32 findUnlockShineNum(bool* out_is_game_clear, GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->findUnlockShineNumCurrentWorld(out_is_game_clear);
}

s32 findUnlockShineNumByWorldId(bool* out_is_game_clear, GameDataHolderAccessor accessor,
                                s32 world_id) {
    return accessor->findUnlockShineNum(out_is_game_clear, world_id);
}

bool isUnlockedWorld(GameDataHolderAccessor accessor, s32 world_id) {
    return accessor->getGameDataFile()->isUnlockedWorld(world_id);
}

bool isUnlockedNextWorld(GameDataHolderAccessor accessor) {
    bool is_game_clear = false;
    s32 unlock_shine_num = findUnlockShineNum(&is_game_clear, accessor);
    return is_game_clear || getPayShineNum(accessor) >= unlock_shine_num;
}

bool isUnlockedAllWorld(GameDataHolderAccessor accessor) {
    return isUnlockedWorld(accessor, getWorldIndexSpecial2());
}

bool isUnlockedCurrentWorld(GameDataHolderAccessor accessor) {
    return isUnlockedWorld(accessor, getCurrentWorldIdNoDevelop(accessor));
}

bool isCollectShineForNextWorldAndNotUnlockNextWorld(const al::LiveActor* actor) {
    return !isUnlockedNextWorld(actor) && rs::checkGetEnoughShineInCurrentWorld(actor);
}

bool isAlreadyGoWorld(GameDataHolderAccessor accessor, s32 world_id) {
    return accessor->getGameDataFile()->isAlreadyGoWorld(world_id);
}

bool isMainStage(GameDataHolderAccessor accessor) {
    return al::isEqualString(getMainStageName(accessor, getCurrentWorldId(accessor)),
                             getCurrentStageName(accessor));
}

const char* getMainStageName(GameDataHolderAccessor accessor, s32 world_id) {
    return accessor->getWorldList()->getMainStageName(world_id);
}

const char* tryGetCurrentMainStageName(GameDataHolderAccessor accessor) {
    s32 world_id = getCurrentWorldId(accessor);
    if (world_id >= 1) {
        const char* stage_name = getMainStageName(accessor, world_id);
        if (stage_name && !al::isEqualString(stage_name, ""))
            return stage_name;
    }
    return getMainStageName(accessor, 0);
}

const char16* tryGetWorldName(const al::LayoutActor* layout, s32 world_id) {
    al::StringTmp<128> label;
    label.format("WorldName_%s", getWorldDevelopName(layout, world_id));
    return al::getSystemMessageString(layout, "StageName", label.cstr());
}

const char16* tryGetWorldNameCurrent(const al::LayoutActor* layout) {
    return tryGetWorldName(layout, getCurrentWorldId(layout));
}

const char16* tryGetRegionNameCurrent(const al::LayoutActor* layout) {
    s32 world_id = getCurrentWorldId(layout);
    al::StringTmp<128> label;
    label.format("RegionName_%s", getWorldDevelopName(layout, world_id));
    return al::getSystemMessageString(layout, "StageName", label.cstr());
}

const char* getWorldDevelopName(GameDataHolderAccessor accessor, s32 world_id) {
    return accessor->getWorldList()->getWorldDevelopName(sead::Mathi::max(world_id, 0));
}

const char* getWorldDevelopNameCurrent(GameDataHolderAccessor accessor) {
    return accessor->getWorldList()->getWorldDevelopName(
        accessor->getGameDataFile()->getCurrentWorldIdNoDevelop());
}

s32 getWorldScenarioNum(GameDataHolderAccessor accessor, s32 world_id) {
    return accessor->getWorldList()->getWorldScenarioNum(sead::Mathi::max(world_id, 0));
}

const char* findMainStageNameByDevelopName(GameDataHolderAccessor accessor, const char* name) {
    for (s32 i = 0; i < getWorldNum(accessor); i++)
        if (al::isEqualString(getWorldDevelopName(accessor, i), name))
            return getMainStageName(accessor, i);
    return nullptr;
}

s32 findWorldIdByDevelopName(GameDataHolderAccessor accessor, const char* name) {
    for (s32 i = 0; i < getWorldNum(accessor); i++)
        if (al::isEqualString(getWorldDevelopName(accessor, i), name))
            return i;
    return 0;
}

static s32 tryFindWorldIdByMainStageNameImpl(GameDataHolderAccessor accessor,
                                             const char* stage_name) {
    return accessor->getWorldList()->tryFindWorldIndexByMainStageName(stage_name);
}

s32 tryFindWorldIdByMainStageName(const al::Scene* scene, const char* stage_name) {
    return tryFindWorldIdByMainStageNameImpl(scene, stage_name);
}

s32 tryFindWorldIdByMainStageName(const al::IUseSceneObjHolder* scene_obj_holder,
                                  const char* stage_name) {
    return tryFindWorldIdByMainStageNameImpl(scene_obj_holder, stage_name);
}

bool checkEnableUnlockWorldSpecial1(const al::LiveActor* actor) {
    if (isUnlockedWorld(actor, getWorldIndexSpecial1()) ||
        isUnlockedWorld(actor, getWorldIndexSpecial2()))
        return false;
    bool is_game_clear = false;
    s32 unlock_shine_num = findUnlockShineNum(&is_game_clear, actor);
    return is_game_clear && getTotalPayShineNum(actor) >= unlock_shine_num;
}

bool checkEnableUnlockWorldSpecial2(const al::LiveActor* actor) {
    if (!isUnlockedWorld(actor, getWorldIndexSpecial1()) ||
        isUnlockedWorld(actor, getWorldIndexSpecial2()))
        return false;
    bool is_game_clear = false;
    s32 unlock_shine_num = findUnlockShineNum(&is_game_clear, actor);
    return is_game_clear && getTotalPayShineNum(actor) >= unlock_shine_num;
}

const char16* tryGetWorldNameByFileId(const al::LayoutActor* layout, s32 file_id) {
    GameDataHolderAccessor accessor{layout};
    s32 world_id = accessor->getGameDataFile(file_id)->getCurrentWorldIdForDisp();
    return tryGetWorldName(layout, world_id);
}

bool isNewSaveDataByFileId(const al::LayoutActor* layout, s32 file_id) {
    GameDataHolderAccessor accessor{layout};
    return accessor->getGameDataFile(file_id)->isPlayDemoOpening();
}

u64 getLastUpdateFileTime(const al::LayoutActor* layout, s32 file_id) {
    GameDataHolderAccessor accessor{layout};
    return accessor->getGameDataFile(file_id)->getLastUpdateTime();
}

static bool isGotShine(const GameDataHolder* game_data_holder, const char* stage_name,
                       const char* obj_id) {
    ShineInfo shine_info;
    shine_info.init(stage_name, obj_id, nullptr, nullptr, ShineInfo::Type_Shine);
    return game_data_holder->getGameDataFile()->isGotShine(&shine_info);
}

void makeTextureSaveDataFileName(sead::BufferedSafeString* out, const nn::g3d::ResFile* res_file,
                                 const GameDataHolder* game_data_holder, s32 file_id) {
    const GameDataFile* game_data_file = game_data_holder->getGameDataFile(file_id);
    s32 world_id = sead::Mathi::clampMin(game_data_file->getCurrentWorldIdForDisp(), 0);
    const char* world_name = game_data_holder->getWorldList()->getWorldDevelopName(world_id);
    s32 main_scenario_no = game_data_file->getMainScenarioNo(world_id);
    s32 scenario_no = game_data_file->getScenarioNo(world_id);

    if (world_id == getWorldIndexPeach()) {
        if (game_data_file->getTotalPayShineNumForDisp() == 999) {
            out->format("PeachWorldCapOn");
            if (agl::g3d::ResFile::GetTextureIndex(res_file, out->cstr()) != -1)
                return;
        }
        out->format("PeachWorld");
        if (agl::g3d::ResFile::GetTextureIndex(res_file, out->cstr()) != -1)
            return;
    }

    if (game_data_holder->getWorldList()->isEqualMoonRockScenarioNo(world_id, scenario_no)) {
        out->format("%sWorldMoonRock", world_name);
        if (agl::g3d::ResFile::GetTextureIndex(res_file, out->cstr()) != -1)
            return;
        out->format("%sWorldED", world_name);
        if (agl::g3d::ResFile::GetTextureIndex(res_file, out->cstr()) != -1)
            return;
        out->format("%sWorld", world_name);
        if (agl::g3d::ResFile::GetTextureIndex(res_file, out->cstr()) != -1)
            return;
        return;
    }

    if (game_data_holder->getWorldList()->isEqualAfterEndingScenarioNo(world_id, scenario_no)) {
        out->format("%sWorldED", world_name);
        if (agl::g3d::ResFile::GetTextureIndex(res_file, out->cstr()) != -1)
            return;
        out->format("%sWorld", world_name);
        if (agl::g3d::ResFile::GetTextureIndex(res_file, out->cstr()) != -1)
            return;
        return;
    }

    if (world_id == getWorldIndexCity()) {
        if (scenario_no == 1) {
            out->format("CityWorld00");
            return;
        }
        if (scenario_no == 2) {
            if (main_scenario_no <= 3) {
                out->format("CityWorld01");
                return;
            }
            out->format("CityWorld02");
            return;
        }
        if (isCityWorldCeremonyAll(world_id, scenario_no)) {
            out->format("CityWorld03");
            return;
        }
        out->format("CityWorld");
        return;
    }

    if (world_id == getWorldIndexSea()) {
        if (scenario_no == 1) {
            if (main_scenario_no == 0) {
                s32 count = 0;
                if (isGotShine(game_data_holder, "SeaWorldHomeStage", "obj382"))
                    count++;
                if (isGotShine(game_data_holder, "SeaWorldLighthouseZone", "obj16"))
                    count++;
                if (isGotShine(game_data_holder, "SeaWorldLavaZone", "obj168"))
                    count++;
                if (isGotShine(game_data_holder, "SeaWorldDamageBallZone", "obj60"))
                    count++;
                if (count == 0) {
                    out->format("SeaWorld00");
                    return;
                }
                out->format("SeaWorld01");
                return;
            }
            if (main_scenario_no == 1) {
                out->format("SeaWorld02");
                return;
            }
        }
        out->format("SeaWorld");
        return;
    }

    if (game_data_file->isPlayDemoOpening()) {
        out->format("Empty");
        return;
    }

    if (main_scenario_no >= 0) {
        out->format("%sWorld%02d", world_name, main_scenario_no);
        if (agl::g3d::ResFile::GetTextureIndex(res_file, out->cstr()) != -1)
            return;
    }

    out->format("%sWorld", world_name);
    if (agl::g3d::ResFile::GetTextureIndex(res_file, out->cstr()) != -1)
        return;
}

void unlockWorld(GameDataHolderWriter writer, s32 world_id) {
    writer->getGameDataFile()->unlockWorld(world_id);
}

static s32 getUnlockWorldIdForWorldMapImpl(GameDataHolderAccessor accessor, s32 world_id) {
    return accessor->getGameDataFile()->getGameProgressData()->getWorldIdForWorldMap(world_id);
}

s32 getUnlockWorldIdForWorldMap(const al::LayoutActor* layout, s32 world_id) {
    return getUnlockWorldIdForWorldMapImpl(layout, world_id);
}

s32 getUnlockWorldIdForWorldMap(const al::LiveActor* actor, s32 world_id) {
    return getUnlockWorldIdForWorldMapImpl(actor, world_id);
}

s32 getUnlockWorldIdForWorldMap(const al::Scene* scene, s32 world_id) {
    return getUnlockWorldIdForWorldMapImpl(scene, world_id);
}

s32 getUnlockWorldIdForWorldMap(const GameDataHolder* game_data_holder, s32 world_id) {
    return game_data_holder->getGameDataFile()->getGameProgressData()->getWorldIdForWorldMap(
        world_id);
}

static s32 getUnlockWorldNumForWorldMapImpl(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getGameProgressData()->getUnlockWorldNum();
}

s32 getUnlockWorldNumForWorldMap(const al::Scene* scene) {
    return getUnlockWorldNumForWorldMapImpl(scene);
}

s32 getUnlockWorldNumForWorldMap(const al::LiveActor* actor) {
    return getUnlockWorldNumForWorldMapImpl(actor);
}

s32 getUnlockWorldNumForWorldMap(const al::LayoutActor* layout) {
    return getUnlockWorldNumForWorldMapImpl(layout);
}

static s32 calcNextLockedWorldIdForWorldMapImpl(GameDataHolderAccessor accessor, s32 world_id) {
    return accessor->getGameDataFile()->getGameProgressData()->calcNextLockedWorldIdForWorldMap(
        world_id);
}

s32 calcNextLockedWorldIdForWorldMap(const al::LayoutActor* layout, s32 world_id) {
    return calcNextLockedWorldIdForWorldMapImpl(layout, world_id);
}

s32 calcNextLockedWorldIdForWorldMap(const al::Scene* scene, s32 world_id) {
    return calcNextLockedWorldIdForWorldMapImpl(scene, world_id);
}

static s32 calcNextLockedWorldNumForWorldMapImpl(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getGameProgressData()->calcNextLockedWorldNumForWorldMap();
}

s32 calcNextLockedWorldNumForWorldMap(const al::Scene* scene) {
    return calcNextLockedWorldNumForWorldMapImpl(scene);
}

s32 calcNextLockedWorldNumForWorldMap(const al::LayoutActor* layout) {
    return calcNextLockedWorldNumForWorldMapImpl(layout);
}

s32 getWorldIdForShineList(const al::LayoutActor* layout, s32 world_id) {
    GameDataHolderAccessor accessor{layout};
    return accessor->getGameDataFile()->getGameProgressData()->getWorldIdForShineList(world_id);
}

s32 calcWorldNumForShineList(const al::LayoutActor* layout) {
    GameDataHolderAccessor accessor{layout};
    return accessor->getGameDataFile()->getGameProgressData()->calcWorldNumForShineList();
}

s32 getLatestUnlockWorldIdForShineTowerMeter(const al::LiveActor* actor) {
    GameDataHolderAccessor accessor{actor};
    return accessor->getGameDataFile()->getGameProgressData()->getWorldIdForWorldWarpHole(
        getUnlockWorldNumForWorldMapImpl(accessor) - 1);
}

bool isClearSandWorldScenario1(const al::Scene* scene) {
    GameDataHolderAccessor accessor{scene};
    s32 world_id = accessor->getWorldList()->tryFindWorldIndexByDevelopName("Sand");
    return getWorldScenarioNo(scene, world_id) > 1;
}

bool isClearCityWorldScenario1(const al::Scene* scene) {
    GameDataHolderAccessor accessor{scene};
    s32 world_id = accessor->getWorldList()->tryFindWorldIndexByDevelopName("City");
    return getWorldScenarioNo(scene, world_id) > 1;
}

bool isClearSkyWorldScenario1(const al::Scene* scene) {
    GameDataHolderAccessor accessor{scene};
    s32 world_id = accessor->getWorldList()->tryFindWorldIndexByDevelopName("Sky");
    return getWorldScenarioNo(scene, world_id) > 1;
}

bool isCityWorldScenario2(const al::IUseSceneObjHolder* scene_obj_holder) {
    if (isWorldCity(scene_obj_holder) && rs::getActiveQuestNum(scene_obj_holder) > 0) {
        s32 quest_no = rs::getActiveQuestNo(scene_obj_holder);
        if (quest_no >= 2 && quest_no <= 6)
            return true;
    }
    return false;
}

s32 calcNextWorldId(GameDataHolderAccessor accessor) {
    return accessor->getWorldList()->tryFindWorldIndexByMainStageName(accessor->getNextStageName());
}

bool isPlayDemoWorldWarp(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isPlayDemoWorldWarp();
}

bool isPlayDemoWorldWarpHole(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isPlayDemoWorldWarpHole();
}

void noPlayDemoWorldWarp(GameDataHolderWriter writer) {
    writer->getGameDataFile()->noPlayDemoWorldWarp();
}

bool isPlayDemoReturnToHome(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isPlayDemoReturnToHome();
}

void requestPlayDemoReturnToHome(GameDataHolderWriter writer) {
    writer->getGameDataFile()->requestPlayDemoReturnToHome();
}

bool isPlayDemoAwardSpecial(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isPlayDemoAwardSpecial();
}

void requestPlayDemoAwardSpecial(GameDataHolderWriter writer) {
    writer->getGameDataFile()->requestPlayDemoAwardSpecial();
}

bool isPlayDemoLavaErupt(GameDataHolderAccessor accessor) {
    return accessor->isPlayDemoLavaErupt();
}

bool isPlayDemoPlayerDownForBattleKoopaAfter(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isPlayDemoPlayerDownForBattleKoopaAfter();
}

void disablePlayDemoPlayerDownForBattleKoopaAfter(GameDataHolderWriter writer) {
    writer->getGameDataFile()->disablePlayDemoPlayerDownForBattleKoopaAfter();
}

void enteredStage(GameDataHolderWriter writer) {
    writer->getGameDataFile()->enteredStage();
    writer->resetTemporaryScenarioCameraHolder();
}

const char* getCurrentCostumeTypeName(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getCurrentCostumeName();
}

const char* getCurrentCapTypeName(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getCurrentCapName();
}

void wearCostume(GameDataHolderWriter writer, const char* name) {
    writer->getGameDataFile()->wearCostume(name);
    rs::trySavePrepoChangeClothEvent(name, getPlayTimeTotal(writer), getSaveDataIdForPrepo(writer),
                                     getPlayTimeAcrossFile(writer));
}

void wearCap(GameDataHolderWriter writer, const char* name) {
    writer->getGameDataFile()->wearCap(name);
    rs::trySavePrepoChangeCapEvent(name, getPlayTimeTotal(writer), getSaveDataIdForPrepo(writer),
                                   getPlayTimeAcrossFile(writer));
}

void wearCostumeRandom(al::IUseSceneObjHolder* scene_obj_holder) {
    const char* current = getCurrentCostumeTypeName(scene_obj_holder);
    const char* target = "Mario";
    s32 total_num = rs::calcHaveClothNum(scene_obj_holder);
    if (total_num != 1) {
        s32 seen_num = 1;
        for (s32 i = 0; i < rs::getClothListSize(scene_obj_holder); i++) {
            if (!rs::isHaveCloth(scene_obj_holder, i) ||
                al::isEqualString(current, rs::getClothList(scene_obj_holder)[i]->name))
                continue;
            f32 chance = 100.0f / (total_num - seen_num++);
            if (al::getRandom(0.0f, 100.0f) < chance) {
                target = rs::getClothList(scene_obj_holder)[i]->name;
                break;
            }
        }
    }
    GameDataHolderAccessor accessor{scene_obj_holder};
    accessor->getGameDataFile()->wearCostume(target);
}

void wearCapRandom(al::IUseSceneObjHolder* scene_obj_holder) {
    const char* current = getCurrentCapTypeName(scene_obj_holder);
    const char* target = "Mario";
    s32 total_num = rs::calcHaveCapNum(scene_obj_holder);
    if (total_num != 1) {
        s32 seen_num = 1;
        for (s32 i = 0; i < rs::getCapListSize(scene_obj_holder); i++) {
            if (!rs::isHaveCap(scene_obj_holder, i) ||
                al::isEqualString(current, rs::getClothList(scene_obj_holder)[i]->name))
                continue;
            f32 chance = 100.0f / (total_num - seen_num++);
            if (al::getRandom(0.0f, 100.0f) < chance) {
                target = rs::getCapList(scene_obj_holder)[i]->name;
                break;
            }
        }
    }
    GameDataHolderAccessor accessor{scene_obj_holder};
    accessor->getGameDataFile()->wearCap(target);
}

void tryWearCostumeRandomIfEnable(al::IUseSceneObjHolder* scene_obj_holder) {
    if (isCostumeRandomMode(scene_obj_holder))
        wearCostumeRandom(scene_obj_holder);
}

void tryWearCapRandomIfEnable(al::IUseSceneObjHolder* scene_obj_holder) {
    if (isCapRandomMode(scene_obj_holder))
        wearCapRandom(scene_obj_holder);
}

bool isCostumeRandomMode(al::IUseSceneObjHolder* scene_obj_holder) {
    GameDataHolderAccessor accessor{scene_obj_holder};
    return accessor->getGameDataFile()->isCostumeRandom();
}

bool isCapRandomMode(al::IUseSceneObjHolder* scene_obj_holder) {
    GameDataHolderAccessor accessor{scene_obj_holder};
    return accessor->getGameDataFile()->isCapRandom();
}

void setCostumeRandomMode(al::IUseSceneObjHolder* scene_obj_holder) {
    GameDataHolderAccessor accessor{scene_obj_holder};
    accessor->getGameDataFile()->setCostumeRandom(true);
    rs::trySavePrepoChangeClothEvent("Random", getPlayTimeTotal(scene_obj_holder),
                                     getSaveDataIdForPrepo(scene_obj_holder),
                                     getPlayTimeAcrossFile(scene_obj_holder));
}

void setCapRandomMode(al::IUseSceneObjHolder* scene_obj_holder) {
    GameDataHolderAccessor accessor{scene_obj_holder};
    accessor->getGameDataFile()->setCapRandom(true);
    rs::trySavePrepoChangeCapEvent("Random", getPlayTimeTotal(scene_obj_holder),
                                   getSaveDataIdForPrepo(scene_obj_holder),
                                   getPlayTimeAcrossFile(scene_obj_holder));
}

void resetCostumeRandomMode(al::IUseSceneObjHolder* scene_obj_holder) {
    GameDataHolderAccessor accessor{scene_obj_holder};
    accessor->getGameDataFile()->setCostumeRandom(false);
}

void resetCapRandomMode(al::IUseSceneObjHolder* scene_obj_holder) {
    GameDataHolderAccessor accessor{scene_obj_holder};
    accessor->getGameDataFile()->setCapRandom(false);
}

const sead::PtrArray<ShopItem::ShopItemInfo>& getShopItemInfoList(GameDataHolderAccessor accessor) {
    return *&accessor->getShopItemInfoList();
}

bool isExistHackObjInfo(GameDataHolderAccessor accessor, const char* hack_name) {
    return getHackObjInfo(accessor, hack_name) != nullptr;
}

const HackObjInfo* getHackObjInfo(GameDataHolderAccessor accessor, const char* hack_name) {
    const sead::PtrArray<HackObjInfo>& list = accessor->getHackObjList();
    for (s32 i = 0; i < list.size(); i++)
        if (al::isEqualString(list.unsafeAt(i)->hackName, hack_name))
            return list.unsafeAt(i);
    return nullptr;
}

void addHackDictionary(GameDataHolderWriter writer, const char* hack_name) {
    writer->getGameDataFile()->addHackDictionary(hack_name);
}

bool isExistInHackDictionary(GameDataHolderAccessor accessor, const char* hack_name) {
    return accessor->getGameDataFile()->isExistInHackDictionary(hack_name);
}

bool isExistInHackDictionary(GameDataHolderAccessor accessor, const char* hack_name, s32 file_id) {
    return accessor->getGameDataFile(file_id)->isExistInHackDictionary(hack_name);
}

bool isShowHackTutorial(GameDataHolderAccessor accessor, const char* hack_name,
                        const char* suffix) {
    return accessor->isShowHackTutorial(hack_name, suffix);
}

void setShowHackTutorial(GameDataHolderWriter writer, const char* hack_name, const char* suffix) {
    writer->setShowHackTutorial(hack_name, suffix);
}

bool isShowBindTutorial(GameDataHolderAccessor accessor, const BindInfo& bind_info) {
    return accessor->isShowBindTutorial(bind_info.name);
}

void setShowBindTutorial(GameDataHolderWriter writer, const BindInfo& bind_info) {
    writer->setShowBindTutorial(bind_info.name);
}

bool isGotShine(GameDataHolderAccessor accessor, s32 world_id, s32 index) {
    return accessor->getGameDataFile()->isGotShine(world_id, index);
}

bool isOpenShineName(const al::LayoutActor* layout, s32 world_id, s32 index) {
    GameDataHolderAccessor accessor{layout};
    return accessor->getGameDataFile()->isOpenShineName(world_id, index);
}

bool checkAchievementShine(const al::LayoutActor* layout, s32 world_id, s32 index) {
    GameDataHolderAccessor accessor{layout};
    return accessor->getGameDataFile()->checkAchievementShine(world_id, index);
}

s32 calcShineNumInOneShine(const al::LayoutActor* layout, s32 world_id, s32 index) {
    GameDataHolderAccessor accessor{layout};
    return sead::Mathi::clampMax(
        accessor->getGameDataFile()->calcShineNumInOneShine(world_id, index), 99);
}

ALWAYS_INLINE static const char16* getShineMessage(const al::IUseSceneObjHolder* scene_obj_holder,
                                                   const al::IUseMessageSystem* message_system,
                                                   s32 world_id, s32 index) {
    const GameDataFile::HintInfo* info =
        getGameDataHolder(scene_obj_holder)->getGameDataFile()->findShine(world_id, index);
    al::StringTmp<128> label;
    al::PlacementId placement_id{info->objId.cstr(), nullptr, nullptr};
    rs::makeMessageLabel(&label, &placement_id, "ScenarioName");
    return al::isExistLabelInStageMessage(message_system, info->stageName.cstr(), label.cstr()) ?
               al::getStageMessageString(message_system, info->stageName.cstr(), label.cstr()) :
               u"シャイン名未設定";
}

const char16* tryFindShineMessage(const al::LayoutActor* layout, s32 world_id, s32 index) {
    return getShineMessage(layout, layout, world_id, index);
}

const char16* tryFindShineMessage(const al::LiveActor* actor,
                                  const al::IUseMessageSystem* message_system, s32 world_id,
                                  s32 index) {
    return getShineMessage(actor, message_system, world_id, index);
}

u64 findShineGetTime(const al::LayoutActor* layout, s32 world_id, s32 index) {
    return getGameDataHolder(layout)->getGameDataFile()->findShine(world_id, index)->getTime;
}

bool checkMoonRockShineForShineList(const al::LayoutActor* layout, s32 world_id, s32 index) {
    return getGameDataHolder(layout)->getGameDataFile()->findShine(world_id, index)->isMoonRock;
}

bool checkUnlockHintByHintNpcForShineList(const al::LayoutActor* layout, s32 world_id, s32 index) {
    return getGameDataHolder(layout)
        ->getGameDataFile()
        ->findShine(world_id, index)
        ->isHintStatusUnlockByNpc();
}

bool checkUnlockHintByAmiiboForShineList(const al::LayoutActor* layout, s32 world_id, s32 index) {
    return getGameDataHolder(layout)
        ->getGameDataFile()
        ->findShine(world_id, index)
        ->isHintStatusUnlockByAmiibo();
}

bool checkShineSeaOfTreeForShineList(const al::LayoutActor* layout, s32 world_id, s32 index) {
    return isSeaOfTreeStage(
        getGameDataHolder(layout)->getGameDataFile()->findShine(world_id, index)->stageName.cstr());
}

s32 getWorldTotalShineNum(GameDataHolderAccessor accessor, s32 world_id) {
    return accessor->getGameDataFile()->getWorldTotalShineNum(world_id);
}

s32 getWorldTotalShineNumMax(GameDataHolderAccessor accessor, s32 world_id) {
    return accessor->getGameDataFile()->getWorldTotalShineNumMax(world_id);
}

void findShineTrans(sead::Vector3f* out, GameDataHolderAccessor accessor, s32 world_id, s32 index) {
    out->set(accessor->getGameDataFile()->findShine(world_id, index)->trans);
}

const sead::Vector3f& getHomeTrans(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getHomeTrans();
}

bool isExistHome(GameDataHolderAccessor accessor) {
    return isGameClear(accessor) || (isActivateHome(accessor) && isUnlockedCurrentWorld(accessor));
}

bool isActivateHome(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getGameProgressData()->isActivateHome();
}

s32 getMiniGameNum(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->calcMiniGameNum();
}

s32 getMiniGameNumMax(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getMiniGameNumMax();
}

const sead::Vector3f& getMiniGameTrans(GameDataHolderAccessor accessor, s32 index) {
    return accessor->getGameDataFile()->getMiniGameTrans(index);
}

const char* getMiniGameName(GameDataHolderAccessor accessor, s32 index) {
    return accessor->getGameDataFile()->getMiniGameName(index);
}

const sead::Vector3f& getRaceStartTrans(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getRaceStartTrans();
}

const sead::Vector3f& getRaceGoalTrans(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getRaceGoalTrans();
}

const sead::Vector3f& getHintNpcTrans(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getHintNpcTrans();
}

const sead::Vector3f& getShopNpcTrans(GameDataHolderAccessor accessor, s32 index) {
    return accessor->getGameDataFile()->getShopNpcTrans(index);
}

bool isShopSellout(GameDataHolderAccessor accessor, s32 index) {
    return accessor->getGameDataFile()->isShopSellout(index);
}

s32 calcShopNum(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->calcShopNum();
}

s32 getShopNpcIconNumMax(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getShopNpcIconNumMax();
}

bool isExistRaceStartNpc(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isExistRaceStartNpc();
}

bool isExistHintNpc(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isExistHintNpc();
}

const sead::Matrix34f& getCurrentMapView(GameDataHolderAccessor accessor) {
    return accessor->getMapDataHolder()->findViewMtx(getCurrentWorldIdNoDevelop(accessor));
}

bool isExistJango(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isExistJango();
}

const sead::Vector3f& getJangoTrans(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getJangoTrans();
}

bool isExistAmiiboNpc(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isExistAmiiboNpc();
}

const sead::Vector3f& getAmiiboNpcTrans(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getAmiiboNpcTrans();
}

bool isInInvalidOpenMapStage(GameDataHolderAccessor accessor) {
    return accessor->isInvalidOpenMapStage(getCurrentStageName(accessor),
                                           getScenarioNoImpl(accessor));
}

const sead::Vector3f& getStageMapPlayerPos(GameDataHolderAccessor accessor) {
    return accessor->getStageMapPlayerPos();
}

bool isExistTimeBalloonNpc(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isExistTimeBalloonNpc();
}

const sead::Vector3f& getTimeBalloonNpcTrans(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getTimeBalloonNpcTrans();
}

bool isExistPoetter(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isExistPoetter();
}

const sead::Vector3f& getPoetterTrans(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getPoetterTrans();
}

const sead::Vector3f& getMoonRockTrans(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getMoonRockTrans();
}

void setHomeTrans(const al::LiveActor* actor, const sead::Vector3f& trans) {
    GameDataHolderAccessor accessor{actor};
    accessor->getGameDataFile()->setHomeTrans(trans);
}

void setRaceStartNpcTrans(const al::LiveActor* actor) {
    GameDataHolderAccessor accessor{actor};
    accessor->getGameDataFile()->setRaceStartNpcTrans(al::getTrans(actor));
}

void setRaceStartTrans(const al::LiveActor* actor) {
    GameDataHolderAccessor accessor{actor};
    accessor->getGameDataFile()->setRaceStartTrans(al::getTrans(actor));
}

void setRaceGoalTrans(const al::LiveActor* actor) {
    GameDataHolderAccessor accessor{actor};
    accessor->getGameDataFile()->setRaceGoalTrans(al::getTrans(actor));
}

void setHintNpcTrans(const al::LiveActor* actor) {
    GameDataHolderAccessor accessor{actor};
    accessor->getGameDataFile()->setHintNpcTrans(al::getTrans(actor));
}

void setJangoTrans(const al::LiveActor* actor) {
    GameDataHolderAccessor accessor{actor};
    accessor->getGameDataFile()->setJangoTrans(al::getTrans(actor));
}

void disableJangoTrans(const al::LiveActor* actor) {
    GameDataHolderAccessor accessor{actor};
    accessor->getGameDataFile()->disableJangoTrans();
}

void setAmiiboNpcTrans(const al::LiveActor* actor) {
    GameDataHolderAccessor accessor{actor};
    accessor->getGameDataFile()->setAmiiboNpcTrans(al::getTrans(actor));
}

void setShopNpcTrans(const al::LiveActor* actor, const char* name, s32 type) {
    GameDataHolderAccessor accessor{actor};
    accessor->getGameDataFile()->setShopNpcTrans(al::getTrans(actor), name, type);
}

void setShopNpcTrans(GameDataHolderAccessor accessor, const al::PlacementInfo& placement_info) {
    sead::Vector3f trans = sead::Vector3f::zero;
    al::getTrans(&trans, placement_info);
    const char* name = nullptr;
    if (al::tryGetStringArg(&name, placement_info, "StoreName"))
        accessor->getGameDataFile()->setShopNpcTrans(trans, name, 0);
}

void setMiniGameInfo(GameDataHolderAccessor accessor, const al::PlacementInfo& placement_info) {
    sead::Vector3f trans = sead::Vector3f::zero;
    al::getTrans(&trans, placement_info);
    const char* name = nullptr;
    if (al::tryGetStringArg(&name, placement_info, "MiniGameName"))
        accessor->getGameDataFile()->setMiniGameInfo(trans, name);
}

void setTimeBalloonTrans(GameDataHolderAccessor accessor, const sead::Vector3f& trans) {
    accessor->getGameDataFile()->setTimeBalloonNpcTrans(trans);
}

void setPoetterTrans(GameDataHolderAccessor accessor, const sead::Vector3f& trans) {
    accessor->getGameDataFile()->setPoetterTrans(trans);
}

void setStageMapPlayerPos(GameDataHolderWriter writer, const sead::Vector3f& pos) {
    writer->setStageMapPlayerPos(pos);
}

void setMoonRockTrans(GameDataHolderWriter writer, const sead::Vector3f& trans) {
    writer->getGameDataFile()->setMoonRockTrans(trans);
}

s32 getHomeLevel(GameDataHolderAccessor accessor) {
    if (rs::isModeE3Rom() || rs::isModeE3LiveRom())
        return 3;
    return accessor->getGameDataFile()->getGameProgressData()->getHomeLevel();
}

void upHomeLevel(GameDataHolderWriter writer) {
    writer->getGameDataFile()->getGameProgressData()->upHomeLevel();
}

void activateHome(GameDataHolderWriter writer) {
    writer->getGameDataFile()->setActivateHome();
}

bool isLaunchHome(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getGameProgressData()->isLaunchHome();
}

void launchHome(GameDataHolderWriter writer) {
    writer->getGameDataFile()->getGameProgressData()->launchHome();
}

bool isCrashHome(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getGameProgressData()->isCrashHome();
}

void crashHome(GameDataHolderWriter writer) {
    writer->getGameDataFile()->getGameProgressData()->crashHome();
}

bool isRepairHome(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getGameProgressData()->isRepairHome();
}

void repairHome(GameDataHolderWriter writer) {
    writer->getGameDataFile()->getGameProgressData()->repairHome();
}

bool isBossAttackedHome(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getGameProgressData()->isBossAttackedHome();
}

void bossAttackHome(GameDataHolderWriter writer) {
    writer->getGameDataFile()->getGameProgressData()->bossAttackHome();
}

bool isRepairHomeByCrashedBoss(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getGameProgressData()->isRepairHomeByCrashedBoss();
}

void repairHomeByCrashedBoss(GameDataHolderWriter writer) {
    writer->getGameDataFile()->getGameProgressData()->repairHomeByCrashedBoss();
}

bool isFindKoopaNext(GameDataHolderAccessor accessor, s32 world_id) {
    return accessor->isFindKoopaNext(world_id);
}

bool isBossAttackedHomeNext(GameDataHolderAccessor accessor, s32 world_id) {
    return accessor->isBossAttackedHomeNext(world_id);
}

bool isFindKoopa(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->getGameProgressData()->isFindKoopa();
}

void findKoopa(GameDataHolderWriter writer) {
    writer->getGameDataFile()->getGameProgressData()->findKoopa();
}

bool isEnableCap(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isEnableCap();
}

bool isMeetCap(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isMeetCap();
}

void enableCap(GameDataHolderWriter writer) {
    writer->getGameDataFile()->meetCap();
}

void disableCapByPlacement(const al::LiveActor* actor) {
    GameDataHolderAccessor accessor{actor};
    accessor->getGameDataFile()->disableCapByPlacement();
}

bool isTalkedCapNearHomeInWaterfall(const al::LiveActor* actor) {
    GameDataHolderAccessor accessor{actor};
    return accessor->getGameDataFile()->getGameProgressData()->isTalkedCapNearHomeInWaterfall();
}

void talkCapNearHomeInWaterfall(const al::LiveActor* actor) {
    GameDataHolderAccessor accessor{actor};
    accessor->getGameDataFile()->getGameProgressData()->talkCapNearHomeInWaterfall();
}

bool isFlagOnTalkMessageInfo(const al::IUseSceneObjHolder* scene_obj_holder, s32 index) {
    GameDataHolderAccessor accessor{scene_obj_holder};
    return accessor->getGameDataFile()->isFlagOnTalkMessageInfo(index);
}

void onFlagTalkMessageInfo(const al::IUseSceneObjHolder* scene_obj_holder, s32 index) {
    GameDataHolderAccessor accessor{scene_obj_holder};
    accessor->getGameDataFile()->setFlagOnTalkMessageInfo(index);
}

s32 getWorldIndexHat() {
    return 0;
}

s32 getWorldIndexWaterfall() {
    return 1;
}

s32 getWorldIndexSand() {
    return 2;
}

s32 getWorldIndexForest() {
    return 3;
}

s32 getWorldIndexLake() {
    return 4;
}

s32 getWorldIndexCloud() {
    return 5;
}

s32 getWorldIndexClash() {
    return 6;
}

s32 getWorldIndexCity() {
    return 7;
}

s32 getWorldIndexSea() {
    return 8;
}

s32 getWorldIndexSnow() {
    return 9;
}

s32 getWorldIndexLava() {
    return 10;
}

s32 getWorldIndexBoss() {
    return 11;
}

s32 getWorldIndexSky() {
    return 12;
}

s32 getWorldIndexMoon() {
    return 13;
}

s32 getWorldIndexPeach() {
    return 14;
}

s32 getWorldIndexSpecial1() {
    return 15;
}

s32 getWorldIndexSpecial2() {
    return 16;
}

bool isWorldCap(GameDataHolderAccessor accessor) {
    return al::isEqualString(getWorldDevelopName(accessor, getCurrentWorldId(accessor)), "Cap");
}

bool isWorldWaterfall(GameDataHolderAccessor accessor) {
    return al::isEqualString(getWorldDevelopName(accessor, getCurrentWorldId(accessor)),
                             "Waterfall");
}

bool isWorldSand(GameDataHolderAccessor accessor) {
    return al::isEqualString(getWorldDevelopName(accessor, getCurrentWorldId(accessor)), "Sand");
}

bool isWorldForest(GameDataHolderAccessor accessor) {
    return al::isEqualString(getWorldDevelopName(accessor, getCurrentWorldId(accessor)), "Forest");
}

bool isWorldLake(GameDataHolderAccessor accessor) {
    return al::isEqualString(getWorldDevelopName(accessor, getCurrentWorldId(accessor)), "Lake");
}

bool isWorldCloud(GameDataHolderAccessor accessor) {
    return al::isEqualString(getWorldDevelopName(accessor, getCurrentWorldId(accessor)), "Cloud");
}

bool isWorldClash(GameDataHolderAccessor accessor) {
    return al::isEqualString(getWorldDevelopName(accessor, getCurrentWorldId(accessor)), "Clash");
}

bool isWorldCity(GameDataHolderAccessor accessor) {
    return al::isEqualString(getWorldDevelopName(accessor, getCurrentWorldId(accessor)), "City");
}

bool isWorldSnow(GameDataHolderAccessor accessor) {
    return al::isEqualString(getWorldDevelopName(accessor, getCurrentWorldId(accessor)), "Snow");
}

bool isWorldSea(GameDataHolderAccessor accessor) {
    return al::isEqualString(getWorldDevelopName(accessor, getCurrentWorldId(accessor)), "Sea");
}

bool isWorldBoss(GameDataHolderAccessor accessor) {
    return al::isEqualString(getWorldDevelopName(accessor, getCurrentWorldId(accessor)), "Attack");
}

bool isWorldSky(GameDataHolderAccessor accessor) {
    return al::isEqualString(getWorldDevelopName(accessor, getCurrentWorldId(accessor)), "Sky");
}

bool isWorldMoon(GameDataHolderAccessor accessor) {
    return al::isEqualString(getWorldDevelopName(accessor, getCurrentWorldId(accessor)), "Moon");
}

bool isWorldPeach(GameDataHolderAccessor accessor) {
    return al::isEqualString(getWorldDevelopName(accessor, getCurrentWorldId(accessor)), "Peach");
}

bool isWorldSpecial1(GameDataHolderAccessor accessor) {
    return al::isEqualString(getWorldDevelopName(accessor, getCurrentWorldId(accessor)),
                             "Special1");
}

bool isWorldSpecial2(GameDataHolderAccessor accessor) {
    return al::isEqualString(getWorldDevelopName(accessor, getCurrentWorldId(accessor)),
                             "Special2");
}

bool isWorldTypeMoon(GameDataHolderAccessor, s32 world_id) {
    return world_id == getWorldIndexMoon() || world_id == getWorldIndexSpecial1() ||
           world_id == getWorldIndexSpecial2();
}

bool isSnowMainScenario1(const al::IUseSceneObjHolder* scene_obj_holder) {
    return rs::getActiveQuestNum(scene_obj_holder) > 0 &&
           rs::getActiveQuestList(scene_obj_holder)[0]->getQuestNo() == 0;
}

bool isSnowMainScenario2(const al::IUseSceneObjHolder* scene_obj_holder) {
    return rs::getActiveQuestNum(scene_obj_holder) > 0 &&
           rs::getActiveQuestList(scene_obj_holder)[0]->getQuestNo() == 1;
}

bool isHomeShipStage(const GameDataHolder* game_data_holder) {
    return al::isEqualString(game_data_holder->getCurrentStageName(), getHomeShipStageName());
}

const char* getHomeShipStageName() {
    return "HomeShipInsideStage";
}

bool isCityWorldCeremonyAll(s32 world_id, s32 scenario_no) {
    if (world_id != getWorldIndexCity())
        return false;
    switch (scenario_no) {
    case 3:
    case 7:
    case 11:
        return true;
    default:
        return false;
    }
}

bool isCityWorldCeremonyAgain(s32 world_id, s32 scenario_no) {
    if (world_id != getWorldIndexCity())
        return false;
    switch (scenario_no) {
    case 7:
    case 11:
        return true;
    default:
        return false;
    }
}

bool isGoToCeremonyFromInsideHomeShip(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isGoToCeremonyFromInsideHomeShip();
}

void registerScenarioStartCameraInfo(const ScenarioStartCamera* actor, s32 id,
                                     al::CameraTicket* camera) {
    getGameDataHolder(actor)->getTemporaryScenarioCameraHolder()->registInfo(id, camera);
}

bool isEnableExplainAmiibo(GameDataHolderAccessor accessor) {
    return accessor->isEnableExplainAmiibo();
}

void endExplainAmiibo(GameDataHolderWriter writer) {
    writer->disableExplainAmiibo();
}

void startSearchHintByAmiibo(GameDataHolderWriter writer) {
    writer->startSearchHintByAmiibo();
}

void endSearchHintByAmiibo(GameDataHolderWriter writer) {
    writer->endSearchHintByAmiibo();
}

void setKidsModeOn(const al::Scene* scene) {
    GameDataHolderAccessor accessor{scene};
    accessor->getGameDataFile()->setKidsMode(true);
}

void setKidsModeOff(const al::Scene* scene) {
    GameDataHolderAccessor accessor{scene};
    accessor->getGameDataFile()->setKidsMode(false);
}

bool isCollectedBgm(GameDataHolderAccessor accessor, const char* name, const char* situation_name) {
    return accessor->getGameDataFile()->isCollectedBgm(name, situation_name);
}

const CollectBgm& getCollectBgmByIndex(GameDataHolderAccessor accessor, s32 index) {
    return accessor->getGameDataFile()->getCollectBgmByIndex(index);
}

bool trySetCollectedBgm(GameDataHolderWriter writer, const char* name, const char* situation_name) {
    return writer->getGameDataFile()->trySetCollectedBgm(name, situation_name);
}

s32 getCollectedBgmNum(GameDataHolderWriter writer) {
    return writer->getGameDataFile()->getCollectedBgmNum();
}

s32 getCollectedBgmNum(GameDataHolderAccessor accessor, s32 file_id) {
    return accessor->getGameDataFile(file_id)->getCollectedBgmNum();
}

s32 getCollectedBgmMaxNum(GameDataHolderWriter writer) {
    return writer->getGameDataFile()->getCollectedBgmMaxNum();
}

void registerShineInfo(GameDataHolderAccessor accessor, const ShineInfo* info,
                       const sead::Vector3f& trans) {
    accessor->getGameDataFile()->registerShineInfo(info, trans);
}

void setHintTrans(GameDataHolderAccessor accessor, s32 index, const sead::Vector3f& trans) {
    if (isMainStage(accessor))
        accessor->getGameDataFile()->setHintTrans(index, trans);
}

void resetHintTrans(GameDataHolderAccessor accessor, s32 index) {
    accessor->getGameDataFile()->resetHintTrans(index);
}

bool isKoopaLv3(GameDataHolderAccessor accessor) {
    return accessor->getGameDataFile()->isKoopaLv3();
}

}  // namespace GameDataFunction
