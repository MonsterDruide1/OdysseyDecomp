#include "Boss/Mofumofu/MofumofuScrap.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/CollisionObj.h"
#include "Library/Obj/PartsFunction.h"
#include "Library/Stage/StageSwitchKeeper.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(MofumofuScrap, Wait)
NERVE_IMPL(MofumofuScrap, ReactionCap)
NERVE_IMPL(MofumofuScrap, Reaction)

NERVES_MAKE_NOSTRUCT(MofumofuScrap, Wait)
NERVES_MAKE_STRUCT(MofumofuScrap, ReactionCap, Reaction)
}  // namespace

MofumofuScrap::MofumofuScrap(const char* name) : al::LiveActor(name) {}

void MofumofuScrap::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::initNerve(this, &Wait, 0);
    al::startAction(this, "Wait");

    al::CollisionObj* collision =
        al::createCollisionObj(this, info, "Leg", al::getHitSensor(this, "Leg"), nullptr, nullptr);
    collision->makeActorAlive();

    al::trySyncStageSwitchAppearAndKill(this);
}

bool MofumofuScrap::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                               al::HitSensor* self) {
    if (rs::isMsgNpcCapReactionAll(message) || rs::isMsgCapTouchWall(message) ||
        rs::isMsgPlayerRollingWallHitDown(message)) {
        al::setNerve(this, &NrvMofumofuScrap.ReactionCap);
        return true;
    }
    if (rs::isMsgPlayerAndCapHipDropAll(message) && !al::isSensorName(self, "Leg")) {
        al::tryOnStageSwitch(this, "SwitchHipDropOn");
        al::setNerve(this, &NrvMofumofuScrap.Reaction);
    }
    return false;
}

void MofumofuScrap::exeWait() {
    if (al::isFirstStep(this))
        al::tryStartActionIfNotPlaying(this, "Wait");
}

void MofumofuScrap::exeReaction() {
    if (al::isFirstStep(this))
        al::startAction(this, "Reaction");
    al::setNerveAtActionEnd(this, &Wait);
}

void MofumofuScrap::exeReactionCap() {
    if (al::isFirstStep(this))
        al::startAction(this, "ReactionCap");
    al::setNerveAtActionEnd(this, &Wait);
}
