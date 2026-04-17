#include "MapObj/HomeInside.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "System/GameDataFunction.h"

HomeInside::HomeInside(const char* name) : al::LiveActor(name) {}

void HomeInside::init(const al::ActorInitInfo& info) {
    auto* gameDataHolder = reinterpret_cast<GameDataHolder*>(info.actorSceneInfo.gameDataHolder);
    const char* currentStageName = GameDataFunction::getCurrentStageName(gameDataHolder);

    const char* suffix = nullptr;
    if (al::isEqualString(currentStageName, "DemoChangeWorldStage") ||
        al::isEqualString(currentStageName, "DemoChangeWorldBossRaidAttackStage") ||
        al::isEqualString(currentStageName, "DemoChangeWorldFindKoopaShipStage")) {
        suffix = "DemoChangeWorldScene";
    }

    al::initActorSuffix(this, info, suffix);
    makeActorAlive();
}
