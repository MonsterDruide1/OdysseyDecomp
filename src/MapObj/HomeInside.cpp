#include "MapObj/HomeInside.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"

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

NERVE_IMPL(HomeShip, Wait);
NERVES_MAKE_NOSTRUCT(HomeShip, Wait);
}  // namespace

HomeInside::HomeInside(const char* name) : al::LiveActor(name) {}

void HomeInside::init(const al::ActorInitInfo& info) {
    const char* currentStageName =
        GameDataFunction::getCurrentStageName((GameDataHolder*)info.actorSceneInfo.gameDataHolder);

    al::initActorSuffix(this, info, chooseHomeInsideSuffix(currentStageName));
    makeActorAlive();
}

HomeShip::HomeShip(const char* name) : al::LiveActor(name) {}

void HomeShip::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "KoopaShip", nullptr);
    al::initNerve(this, &Wait, 0);
    al::setScaleAll(this, 0.25f);

    al::PlacementInfo placementInfo;
    al::getLinksInfo(&placementInfo, info, "GoalObj");

    mGoalObj = al::createChildLinkSimpleActor("ゴールホーム", "GoalObj", info, true);
    makeActorAlive();
}

bool HomeShip::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    return false;
}

void HomeShip::exeWait() {}
