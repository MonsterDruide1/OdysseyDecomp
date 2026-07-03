#include "Npc/SessionMusicianWarpAgent.h"

#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"

#include "MapObj/ChangeStageInfo.h"
#include "MapObj/PlayerStartInfoHolder.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "Util/DemoUtil.h"

static void registerPlayerRestartPosNormal(const al::PlacementInfo& info, al::LiveActor* actor) {
    al::PlacementInfo restartInfo = info;
    al::getLinksInfo(&restartInfo, info, "PlayerRestartPosNormal");

    rs::registerPlayerStartInfoToHolder(actor, restartInfo, nullptr, nullptr, nullptr, nullptr);
}

static void registerPlayerRestartPosTalk(const al::PlacementInfo& info, al::LiveActor* actor) {
    al::PlacementInfo restartInfo = info;
    al::getLinksInfo(&restartInfo, info, "PlayerRestartPosTalk");

    rs::registerPlayerStartInfoToHolder(actor, restartInfo, nullptr, nullptr, nullptr, nullptr);
}

static ChangeStageInfo* createChangeStageNormal(const al::PlacementInfo& info,
                                                al::LiveActor* actor) {
    const char* stageName = nullptr;
    if (!al::tryGetStringArg(&stageName, info, "ChangeStageName"))
        return nullptr;

    const char* stageId = nullptr;
    if (!al::tryGetStringArg(&stageId, info, "ChangeStageIdNormal"))
        return nullptr;

    return rs::createChangeStageInfo(actor, stageId, stageName, false, -1,
                                     ChangeStageInfo::SubScenarioType::NO_SUB_SCENARIO);
}

static ChangeStageInfo* createChangeStageTalk(const al::PlacementInfo& info, al::LiveActor* actor) {
    const char* stageName = nullptr;
    if (!al::tryGetStringArg(&stageName, info, "ChangeStageName"))
        return nullptr;

    const char* stageId = nullptr;
    if (!al::tryGetStringArg(&stageId, info, "ChangeStageIdTalk"))
        return nullptr;

    return rs::createChangeStageInfo(actor, stageId, stageName, false, -1,
                                     ChangeStageInfo::SubScenarioType::NO_SUB_SCENARIO);
}

SessionMusicianWarpAgent::SessionMusicianWarpAgent(al::LiveActor* actor,
                                                   const al::ActorInitInfo& initInfo)
    : mActor(actor) {
    al::PlacementInfo info;
    al::getLinksInfo(&info, initInfo, "WarpAgent");

    mPlacementInfo = new al::PlacementInfo(info);

    if (al::isExistLinkChild(info, "PlayerRestartPosNormal", 0))
        registerPlayerRestartPosNormal(info, actor);

    if (al::isExistLinkChild(info, "PlayerRestartPosTalk", 0))
        registerPlayerRestartPosTalk(info, actor);

    mChangeStageNormal = createChangeStageNormal(info, actor);
    mChangeStageTalk = createChangeStageTalk(info, actor);

    if (mChangeStageTalk)
        mChangeStageTalk->setWipeType("地下工場");
}

static const char* getWarpTargetInfoName(SessionMusicianWarpAgent* self) {
    if (GameDataFunction::getSessionEventProgress(self->getActor()) >
        SessionEventProgress::Wait4thMusician)
        return "PlayerRestartPosTalk";
    return "PlayerRestartPosNormal";
}

bool SessionMusicianWarpAgent::tryGetWarpTargetInfo(al::PlacementInfo* targetInfo) {
    return al::tryGetLinksInfo(targetInfo, *mPlacementInfo, getWarpTargetInfoName(this));
}

static inline const ChangeStageInfo* getNextStage(SessionMusicianWarpAgent* self) {
    if (GameDataFunction::getSessionEventProgress(self->getActor()) >
        SessionEventProgress::Wait4thMusician)
        return self->getChangeStageTalk();

    return self->getChangeStageNormal();
}

bool SessionMusicianWarpAgent::tryStartWarp() {
    if (!mChangeStageNormal && !mChangeStageTalk)
        return rs::requestStartDemoWarp(mActor);

    return GameDataFunction::tryChangeNextStage(mActor, getNextStage(this));
}
