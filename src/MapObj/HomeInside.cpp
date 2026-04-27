#include "MapObj/HomeInside.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "System/GameDataFunction.h"

namespace {
static inline const char* chooseHomeInsideSuffix(const char* currentStageName) {
    if (!al::isEqualString(currentStageName, "DemoChangeWorldStage") &&
        !al::isEqualString(currentStageName, "DemoChangeWorldBossRaidAttackStage") &&
        !al::isEqualString(currentStageName, "DemoChangeWorldFindKoopaShipStage")) {
        return nullptr;
    }

    return "DemoChangeWorldScene";
}
}  // namespace

HomeInside::HomeInside(const char* name) : al::LiveActor(name) {}

void HomeInside::init(const al::ActorInitInfo& info) {
    const char* currentStageName =
        GameDataFunction::getCurrentStageName((GameDataHolder*)info.actorSceneInfo.gameDataHolder);

    al::initActorSuffix(this, info, chooseHomeInsideSuffix(currentStageName));
    makeActorAlive();
}
