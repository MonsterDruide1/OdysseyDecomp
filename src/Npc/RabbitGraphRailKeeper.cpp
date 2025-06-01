#include "Npc/RabbitGraphRailKeeper.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Rail/Rail.h"
#include "Library/Rail/RailRider.h"
#include "Library/Rail/RailUtil.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

namespace {
NERVE_IMPL(RabbitGraphRailKeeper, Wait);

NERVES_MAKE_STRUCT(RabbitGraphRailKeeper, Wait);
}  // namespace

RabbitGraphRailKeeper::RabbitGraphRailKeeper(const char* name) : al::LiveActor(name) {}

RabbitGraphRailKeeper::RabbitGraphRailKeeper(const char* name, al::LiveActor* actor)
    : al::LiveActor(name) {}

void RabbitGraphRailKeeper::init(const al::ActorInitInfo& initInfo) {
    using RabbitGraphRailKeeperFunctor =
        al::FunctorV0M<RabbitGraphRailKeeper*, void (RabbitGraphRailKeeper::*)()>;

    al::initActorSceneInfo(this, initInfo);
    al::initActorPoseTRSV(this);
    al::initActorSRT(this, initInfo);
    al::initActorClipping(this, initInfo);
    al::initStageSwitch(this, initInfo);

    const char* objectName;
    al::tryGetObjectName(&objectName, initInfo);

    if (al::isObjectName(initInfo, "RailRabbit")) {
        mRail = new al::Rail();
        mRail->init(*initInfo.placementInfo);
        mRailRider = new al::RailRider(mRail);
    } else {
        if (!al::isExistRail(initInfo, "Rail")) {
            makeActorDead();
            return;
        }

        initRailKeeper(initInfo, "Rail");
    }

    al::initExecutorUpdate(this, initInfo, "地形オブジェ[Movement]");
    al::initNerve(this, &NrvRabbitGraphRailKeeper.Wait, 0);
    al::tryGetArg(&mIsJump, initInfo, "IsJump");
    makeActorAlive();

    if (al::listenStageSwitchOnAppear(
            this, RabbitGraphRailKeeperFunctor(this, &RabbitGraphRailKeeper::appearBySwitch)))
        kill();

    al::listenStageSwitchOnKill(
        this, RabbitGraphRailKeeperFunctor(this, &RabbitGraphRailKeeper::killBySwitch));
}

void RabbitGraphRailKeeper::appearBySwitch() {
    if (al::isAlive(this))
        return;
    appear();
}

void RabbitGraphRailKeeper::killBySwitch() {
    if (al::isDead(this))
        return;
    kill();
}

bool RabbitGraphRailKeeper::isRailPointIgnore(s32 index) const {
    al::PlacementInfo* placementInfo = al::getRailPointInfo(this, index);

    bool isIgnore;
    if (!al::tryGetArg(&isIgnore, *placementInfo, "IsIgnore"))
        return false;

    return isIgnore;
}

bool RabbitGraphRailKeeper::isRailPointConnectToCollision(s32 index) const {
    al::PlacementInfo* placementInfo = al::getRailPointInfo(this, index);

    bool isConnectToCollision;
    if (!al::tryGetArg(&isConnectToCollision, *placementInfo, "IsConnectToCollision"))
        return false;

    return isConnectToCollision;
}

void RabbitGraphRailKeeper::appear() {
    al::LiveActor::appear();
}

void RabbitGraphRailKeeper::kill() {
    al::LiveActor::kill();
}

void RabbitGraphRailKeeper::exeWait() {
    al::isFirstStep(this);
}

al::RailRider* RabbitGraphRailKeeper::getRailRider() const {
    if (mRailRider)
        return mRailRider;

    return al::LiveActor::getRailRider();
}
