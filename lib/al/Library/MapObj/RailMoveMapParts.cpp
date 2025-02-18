#include "Library/MapObj/RailMoveMapParts.h"

#include "Library/Area/SwitchKeepOnAreaGroup.h"
#include "Library/Area/SwitchOnAreaGroup.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/Movement/RailMoveMovement.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveStateCtrl.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Rail/RailUtil.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Thread/FunctorV0M.h"

namespace al {
namespace {
NERVE_ACTION_IMPL(RailMoveMapParts, StandBy)
NERVE_ACTION_IMPL(RailMoveMapParts, MoveSign)
NERVE_ACTION_IMPL(RailMoveMapParts, Move)

NERVE_ACTIONS_MAKE_STRUCT(RailMoveMapParts, StandBy, MoveSign, Move)
}  // namespace

RailMoveMapParts::RailMoveMapParts(const char* name) : LiveActor(name) {}

void RailMoveMapParts::init(const ActorInitInfo& info) {
    using RailMoveMapPartsFunctor = FunctorV0M<RailMoveMapParts*, void (RailMoveMapParts::*)()>;

    initNerveAction(this, "StandBy", &NrvRailMoveMapParts.mCollector, 1);
    initMapPartsActor(this, info, nullptr);
    registerAreaHostMtx(this, info);

    if (isExistRail(this)) {
        f32 radius = getClippingRadius(this);
        setSyncRailToNearestPos(this);
        mRailCoord = getRailCoord(this);
        setRailClippingInfo(&mRailPos, this, 100.0f, radius);
    }

    RailMoveMovement* railMoveMovement = new RailMoveMovement(this, info);
    initNerveState(this, railMoveMovement, NrvRailMoveMapParts.Move.data(), "レール移動");
    initMaterialCode(this, info);

    if (!listenStageSwitchOnStart(this, RailMoveMapPartsFunctor(this, &RailMoveMapParts::start)))
        start();

    if (listenStageSwitchOnStop(this, RailMoveMapPartsFunctor(this, &RailMoveMapParts::stop)))
        stop();

    mSwitchKeepOnAreaGroup = tryCreateSwitchKeepOnAreaGroup(this, info);
    mSwitchOnAreaGroup = tryCreateSwitchOnAreaGroup(this, info);

    trySyncStageSwitchAppear(this);
    tryListenStageSwitchKill(this);
}

void RailMoveMapParts::start() {
    if (isNerve(this, NrvRailMoveMapParts.StandBy.data()))
        startNerveAction(this, "MoveSign");
}

void RailMoveMapParts::stop() {
    if (isNerve(this, NrvRailMoveMapParts.Move.data()))
        startNerveAction(this, "StandBy");
}

bool RailMoveMapParts::receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) {
    if (isMsgShowModel(message)) {
        showModelIfHide(this);

        return true;
    }

    if (isMsgHideModel(message)) {
        hideModelIfShow(this);

        return true;
    }

    return false;
}

void RailMoveMapParts::control() {
    if (mSwitchKeepOnAreaGroup != nullptr)
        mSwitchKeepOnAreaGroup->update(getTrans(this));

    if (mSwitchOnAreaGroup != nullptr)
        mSwitchOnAreaGroup->update(getTrans(this));
}

void RailMoveMapParts::appearAndSetStart() {
    setSyncRailToCoord(this, mRailCoord);
    startNerveAction(this, "MoveSign");
}

void RailMoveMapParts::exeStandBy() {}

void RailMoveMapParts::exeMoveSign() {
    if ((isFirstStep(this) && !tryStartAction(this, "MoveSign")) || isActionEnd(this))
        startNerveAction(this, "Move");
}

void RailMoveMapParts::exeMove() {
    updateNerveState(this);
}
}  // namespace al
