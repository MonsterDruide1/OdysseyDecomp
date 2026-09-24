#include "MapObj/ElectricAttackTarget.h"

#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Rail/RailUtil.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(ElectricAttackTarget, Wait);
NERVES_MAKE_NOSTRUCT(ElectricAttackTarget, Wait);
}  // namespace

ElectricAttackTarget::ElectricAttackTarget(const char* name) : al::LiveActor(name) {}

void ElectricAttackTarget::init(const al::ActorInitInfo& info) {
    using ElectricAttackTargetFunctor =
        al::FunctorV0M<ElectricAttackTarget*, void (ElectricAttackTarget::*)()>;

    al::initActor(this, info);
    al::initNerve(this, &Wait, 0);
    al::tryGetArg(&mRailMoveSpeed, info, "RailMoveSpeed");
    mMtxConnector = al::tryCreateMtxConnector(this, info);
    makeActorAlive();

    if (al::listenStageSwitchOnAppear(
            this, ElectricAttackTargetFunctor(this, &ElectricAttackTarget::appearBySwitch)))
        kill();

    al::listenStageSwitchOnKill(
        this, ElectricAttackTargetFunctor(this, &ElectricAttackTarget::killBySwitch));
}

void ElectricAttackTarget::appearBySwitch() {
    if (!al::isAlive(this))
        al::LiveActor::appear();
}

void ElectricAttackTarget::killBySwitch() {
    if (!al::isDead(this))
        al::LiveActor::kill();
}

void ElectricAttackTarget::initAfterPlacement() {
    if (mMtxConnector)
        al::attachMtxConnectorToCollision(mMtxConnector, this, false);
}

bool ElectricAttackTarget::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                      al::HitSensor* self) {
    if (al::isNerve(this, &Wait)) {
        if (rs::isMsgHackAttack(message))
            return true;

        if (rs::isMsgConductLightning(message))
            return true;
    }

    return false;
}

void ElectricAttackTarget::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");

    if (al::isExistRail(this)) {
        if (al::isLoopRail(this))
            al::moveSyncRailLoop(this, mRailMoveSpeed);
        else
            al::moveSyncRailTurn(this, mRailMoveSpeed);
    }

    if (mMtxConnector && al::isMtxConnectorConnecting(mMtxConnector))
        al::connectPoseQT(this, mMtxConnector);
}
