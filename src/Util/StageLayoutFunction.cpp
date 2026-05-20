#include "Util/StageLayoutFunction.h"

#include <basis/seadTypes.h>

#include "Library/Base/StringUtil.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementId.h"

#include "System/CollectBgm.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "System/GameDataUtil.h"
#include "Util/SequentialUtil.h"

namespace {
const char16 cEmptyMessage[] = {0};
}  // namespace

namespace rs {

const char16* searchNpcMessage(const al::ActorInitInfo& initInfo, const al::LayoutActor* layout) {
    al::StringTmp<256> label;
    makeActorMessageLabel(&label, initInfo, nullptr);

    const char* stageName = GameDataFunction::getCurrentStageName(GameDataHolderAccessor(layout));
    if (!al::isExistStageMessage(layout, stageName))
        return nullptr;
    if (!al::isExistLabelInStageMessage(layout, stageName, label.cstr()))
        return nullptr;
    return al::getStageMessageString(layout, stageName, label.cstr());
}

void makeActorMessageLabel(sead::BufferedSafeString* out, const al::ActorInitInfo& initInfo,
                           const char* objectName) {
    const al::PlacementInfo& placementInfo = al::getPlacementInfo(initInfo);
    const char* labelName = objectName;
    if (!labelName)
        al::getObjectName(&labelName, placementInfo);
    al::PlacementId placementId;
    al::getPlacementId(&placementId, placementInfo);
    out->format("%s_%s", labelName, placementId.getId());
}

void makeMessageLabel(sead::BufferedSafeString* out, const al::PlacementInfo& placementInfo,
                      const char* objectName) {
    const char* labelName = objectName;
    if (!labelName)
        al::getObjectName(&labelName, placementInfo);
    al::PlacementId placementId;
    al::getPlacementId(&placementId, placementInfo);
    out->format("%s_%s", labelName, placementId.getId());
}

void makeMessageLabel(sead::BufferedSafeString* out, const al::PlacementId* placementId,
                      const char* objectName) {
    out->format("%s_%s", objectName, placementId->getId());
}

void setPaneStageMessageActorLabel(al::LayoutActor* layout, const char* paneName,
                                   const al::ActorInitInfo& initInfo, const char* objectName) {
    al::StringTmp<128> label;
    makeActorMessageLabel(&label, initInfo, objectName);
    const char* labelStr = label.cstr();

    GameDataHolder* holder = GameDataHolderAccessor(layout);
    const al::PlacementInfo& placementInfo = al::getPlacementInfo(initInfo);
    const char* stageName;
    {
        const char* zoneName = nullptr;
        if (al::tryGetZoneNameIfExist(&zoneName, placementInfo))
            stageName = zoneName;
        else
            stageName = GameDataFunction::getCurrentStageName(holder);
    }
    if (!stageName) {
        GameDataHolderAccessor fallbackAccessor(layout);
        stageName = GameDataFunction::getCurrentStageName(fallbackAccessor);
    }
    if (al::isExistLabelInStageMessage(layout, stageName, labelStr))
        al::setPaneStageMessage(layout, paneName, stageName, labelStr);
}

bool trySetPaneStageMessageIfExist(al::LayoutActor* layout, const char* paneName, const char* label,
                                   const char* stageName) {
    const char* messageStageName = stageName;
    if (!messageStageName)
        messageStageName = GameDataFunction::getCurrentStageName(GameDataHolderAccessor(layout));
    if (!al::isExistLabelInStageMessage(layout, messageStageName, label))
        return false;
    al::setPaneStageMessage(layout, paneName, messageStageName, label);
    return true;
}

const char* getPlacementStageName(GameDataHolderAccessor accessor,
                                  const al::ActorInitInfo& initInfo) {
    const al::PlacementInfo& placementInfo = al::getPlacementInfo(initInfo);
    const char* stageName = nullptr;
    if (al::tryGetZoneNameIfExist(&stageName, placementInfo))
        return stageName;
    return GameDataFunction::getCurrentStageName(accessor);
}

const char16* getStageMessageActorLabel(al::LayoutActor* layout, const al::ActorInitInfo& initInfo,
                                        const char* objectName) {
    al::StringTmp<128> label;
    makeActorMessageLabel(&label, initInfo, objectName);

    GameDataHolder* holder = GameDataHolderAccessor(layout);
    const al::PlacementInfo& placementInfo = al::getPlacementInfo(initInfo);
    const char* zoneName = nullptr;
    const char* stageName;
    if (al::tryGetZoneNameIfExist(&zoneName, placementInfo))
        stageName = zoneName;
    else
        stageName = GameDataFunction::getCurrentStageName(holder);
    if (!al::isExistLabelInStageMessage(layout, stageName, label.cstr()))
        return cEmptyMessage;
    return al::getStageMessageString(layout, stageName, label.cstr());
}

bool isExistLabelInSystemMessageWithFileName(const al::IUseMessageSystem* messageSystem,
                                             const char* fileName, const char* label) {
    return al::isExistLabelInSystemMessage(messageSystem, fileName, label);
}

bool isExistLabelInStageMessageWithFileName(const al::IUseMessageSystem* messageSystem,
                                            const char* fileName, const char* label) {
    return al::isExistLabelInStageMessage(messageSystem, fileName, label);
}

bool trySetPaneSystemMessageIfExist(al::LayoutActor* layout, const char* paneName,
                                    const char* fileName, const char* label) {
    if (!al::isExistLabelInSystemMessage(layout, fileName, label))
        return false;
    al::setPaneSystemMessage(layout, paneName, fileName, label);
    return true;
}

bool trySetPaneStringCheckpointName(al::LayoutActor* layout, const char* paneName,
                                    s32 checkpointIndex, const char* stageName) {
    const char* messageStageName = stageName;
    if (!messageStageName)
        messageStageName = GameDataFunction::getCurrentStageName(GameDataHolderAccessor(layout));

    al::StringTmp<128> label;
    label.format("%s_%s", "Checkpoint",
                 GameDataFunction::getCheckpointObjIdInWorld(GameDataHolderAccessor(layout),
                                                             checkpointIndex));
    if (!al::isExistLabelInStageMessage(layout, messageStageName, label.cstr()))
        return false;
    al::setPaneStageMessage(layout, paneName, messageStageName, label.cstr());
    return true;
}

const char* getCheckpointLabelPrefix() {
    return "Checkpoint";
}

void setPaneStringWorldNameForWorldMap(al::LayoutActor* layout, const char* paneName, s32 worldId) {
    const char16* worldName = GameDataFunction::tryGetWorldName(layout, worldId);
    al::setPaneString(layout, paneName, worldName);
}

void setPaneStringWorldNameForWorldMapWithoutRuby(al::LayoutActor* layout, const char* paneName,
                                                  s32 worldId) {
    char16 message[256];
    al::copyMessageWithoutTag(message, 256, GameDataFunction::tryGetWorldName(layout, worldId));
    al::setPaneString(layout, paneName, message);
}

void setRaceRecordMessage(al::LayoutActor* layout, const char* paneName, s32 recordFrame) {
    s32 minutes = recordFrame / 3600;
    s32 framesInHour = recordFrame + minutes * -3600;
    s32 seconds = framesInHour / 60 % 60;
    s32 centiseconds = ((framesInHour + seconds * -60) % 60) * 100 / 60;

    al::WStringTmp<256> message;
    al::ReplaceTimeInfo replaceTimeInfo;
    al::createReplaceTimeInfoForRaceTime(&replaceTimeInfo, minutes, seconds, centiseconds);
    al::replaceMessageTagTimeDirectRaceTime(&message, layout, replaceTimeInfo);
    al::setPaneString(layout, paneName, message.cstr());
}

void setRaceRecordMessageCsec(al::LayoutActor* layout, const char* paneName, s32 recordCsec) {
    s32 clampedCsec = RaceTimeFunction::clampRaceRecordCsec(recordCsec);
    s32 secondsTotal = clampedCsec / 100;
    s32 minutes = clampedCsec / 6000;
    s32 seconds = secondsTotal + minutes * -60;
    s32 centiseconds = clampedCsec % 100;

    al::WStringTmp<256> message;
    al::ReplaceTimeInfo replaceTimeInfo;
    al::createReplaceTimeInfoForRaceTime(&replaceTimeInfo, minutes, seconds, centiseconds);
    al::replaceMessageTagTimeDirectRaceTime(&message, layout, replaceTimeInfo);
    al::setPaneString(layout, paneName, message.cstr());
}

void replaceRaceRecordMessageCsec(al::LayoutActor* layout, const char* paneName, s32 recordCsec,
                                  const char16* messageTag, const char* tagName) {
    s32 secondsTotal = recordCsec / 100;
    s32 minutes = recordCsec / 6000;
    s32 seconds = secondsTotal + minutes * -60;
    s32 centiseconds = recordCsec % 100;

    al::WStringTmp<256> message;
    al::ReplaceTimeInfo replaceTimeInfo;
    al::createReplaceTimeInfoForRaceTime(&replaceTimeInfo, minutes, seconds, centiseconds);
    al::replaceMessageTagTimeDirectRaceTime(&message, layout, replaceTimeInfo);
    al::replaceMessageTagTime(&message, layout, messageTag, replaceTimeInfo, tagName);
    al::setPaneString(layout, paneName, message.cstr());
}

void setRaceNoRecordMessage(al::LayoutActor* layout, const char* paneName) {
    al::setPaneString(layout, paneName, al::getSystemMessageString(layout, "Time", "NoRaceTime"));
}

void setMiniGameNoCountMessage(al::LayoutActor* layout, const char* paneName) {
    al::setPaneString(layout, paneName, al::getSystemMessageString(layout, "MiniGame", "NoCount"));
}

const char16* getWorldShinePictureFont(const al::LayoutActor* layout, s32 worldId,
                                       bool isUseCompleteIcon) {
    al::StringTmp<128> label;
    label.format("ShineIcon_%s",
                 GameDataFunction::getWorldDevelopName(GameDataHolderAccessor(layout), worldId));
    if (isUseCompleteIcon && GameDataFunction::isGameClear(GameDataHolderAccessor(layout)))
        label.format("ShineIcon_All");
    return al::getSystemMessageString(layout, "IconTag", label.cstr());
}

const char16* getNullShinePictureFont(const al::LayoutActor* layout) {
    al::StringTmp<128> label;
    label.format("ShineIcon_Null");
    return al::getSystemMessageString(layout, "IconTag", label.cstr());
}

const char16* getWorldCoinCollectPictureFont(const al::LayoutActor* layout, s32 worldId) {
    s32 iconWorldId = worldId;
    if (GameDataFunction::getCoinCollectNumMax(GameDataHolderAccessor(layout), worldId) == 0)
        iconWorldId = 0;

    al::StringTmp<128> label;
    label.format("CoinCollectIcon_%s", GameDataFunction::getWorldDevelopName(
                                           GameDataHolderAccessor(layout), iconWorldId));
    return al::getSystemMessageString(layout, "IconTag", label.cstr());
}

const char16* getWorldCoinCollectPictureFont(const al::LayoutActor* layout) {
    return getWorldCoinCollectPictureFont(
        layout, GameDataFunction::getCurrentWorldId(GameDataHolderAccessor(layout)));
}

const char* getPlacementStageName(GameDataHolderAccessor accessor,
                                  const al::PlacementInfo& placementInfo) {
    const char* stageName = nullptr;
    if (al::tryGetZoneNameIfExist(&stageName, placementInfo))
        return stageName;
    return GameDataFunction::getCurrentStageName(accessor);
}

const char16* getCollectBgmMessage(const al::IUseMessageSystem* messageSystem,
                                   const CollectBgm* bgm) {
    al::StringTmp<256> label;
    if (bgm->situationName)
        label.format("%s@%s", bgm->name, bgm->situationName);
    else
        label.format("%s", bgm->name);

    if (!al::isExistLabelInSystemMessage(messageSystem, "CollectBgmList", label.cstr()))
        return nullptr;
    return al::getSystemMessageString(messageSystem, "CollectBgmList", label.cstr());
}

void setPaneCurrentCoinNum(al::LayoutActor* layout) {
    al::setPaneStringFormat(layout, "TxtCounter", "%04d",
                            GameDataFunction::getCoinNum(GameDataHolderAccessor(layout)));
}

bool tryGetMapMainScenarioLabel(sead::BufferedSafeString* outLabel,
                                sead::BufferedSafeString* outStageName, bool* isNextScenario,
                                const al::IUseSceneObjHolder* holder) {
    GameDataHolderAccessor accessor(holder);
    if (GameDataFunction::isExistJango(accessor)) {
        outStageName->format("%s", "StageMapMessage");
        outLabel->format("TxtJango");
        *isNextScenario = false;
        return true;
    }

    if (isSequenceCollectShineForRepairHome(holder)) {
        outStageName->format("%s", "StageMapMessage");
        outLabel->format("TxtCollectShineForRepairHome");
        *isNextScenario = false;
        return true;
    }

    if (isSequenceGoToMoonRock(holder) || isSequenceCollectShine(holder)) {
        outStageName->format("%s", "StageMapMessage");
        outLabel->format("TxtCollectShine");
        *isNextScenario = false;
        return true;
    }

    if (isSequenceGoToNextWorld(holder)) {
        outStageName->format("%s", "StageMapMessage");
        outLabel->format("TxtGoToNextWorld");
        *isNextScenario = false;
        return true;
    }

    if (!GameDataFunction::tryGetNextMainScenarioLabel(outLabel, outStageName, holder))
        return false;
    *isNextScenario = true;
    return true;
}

}  // namespace rs
