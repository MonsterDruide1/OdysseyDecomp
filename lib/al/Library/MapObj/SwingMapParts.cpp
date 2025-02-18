#include "Library/MapObj/SwingMapParts.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/MapObj/ChildStep.h"
#include "Library/Movement/SwingMovement.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Thread/FunctorV0M.h"

namespace {
using namespace al;

NERVE_ACTION_IMPL(SwingMapParts, StandBy)
NERVE_ACTION_IMPL(SwingMapParts, MoveRight)
NERVE_ACTION_IMPL(SwingMapParts, MoveLeft)
NERVE_ACTION_IMPL(SwingMapParts, Stop)

NERVE_ACTIONS_MAKE_STRUCT(SwingMapParts, StandBy, MoveRight, MoveLeft, Stop)
}  // namespace

namespace al {
SwingMapParts::SwingMapParts(const char* name) : LiveActor(name) {}

void SwingMapParts::init(const ActorInitInfo& info) {
    using SwingMapPartsFunctor = FunctorV0M<SwingMapParts*, void (SwingMapParts::*)()>;

    mSwingMovement = new SwingMovement(info);

    if (mSwingMovement->isLeft())
        initNerveAction(this, "MoveLeft", &NrvSwingMapParts.mCollector, 0);
    else
        initNerveAction(this, "MoveRight", &NrvSwingMapParts.mCollector, 0);

    tryInitSubActorKeeperChildStep(this, info);
    initMapPartsActor(this, info, nullptr);
    tryGetQuatPtr(this);
    registerAreaHostMtx(this, info);

    mQuat = getQuat(this);

    createChildStep(info, this, true);
    tryGetArg((s32*)&mRotateAxis, info, "RotateAxis");
    tryGetArg(&mIsFloorTouchStart, info, "IsFloorTouchStart");

    if (mIsFloorTouchStart ||
        listenStageSwitchOnStart(this, SwingMapPartsFunctor(this, &SwingMapParts::start)))
        startNerveAction(this, "StandBy");

    rotateQuatLocalDirDegree(this, mQuat, (s32)mRotateAxis, mSwingMovement->getCurrentAngle());
    trySyncStageSwitchAppear(this);
}

void SwingMapParts::start() {
    if (!isNerve(this, NrvSwingMapParts.StandBy.data()))
        return;

    if (mSwingMovement->isLeft())
        startNerveAction(this, "MoveLeft");
    else
        startNerveAction(this, "MoveRight");
}

void SwingMapParts::initAfterPlacement() {
    tryExpandClippingByDepthShadowLength(this, &mDepthShadowLength);
}

bool SwingMapParts::receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) {
    if (mIsFloorTouchStart && isMsgFloorTouch(message) &&
        isNerve(this, NrvSwingMapParts.StandBy.data())) {
        if (mSwingMovement->isLeft())
            startNerveAction(this, "MoveLeft");
        else
            startNerveAction(this, "MoveRight");

        return true;
    }

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

void SwingMapParts::control() {
    rotateQuatLocalDirDegree(this, mQuat, (s32)mRotateAxis, mSwingMovement->getCurrentAngle());
}

void SwingMapParts::exeStandBy() {}

void SwingMapParts::exeMoveRight() {
    mSwingMovement->updateNerve();

    if (mSwingMovement->isStop())
        startNerveAction(this, "Stop");
}

void SwingMapParts::exeMoveLeft() {
    mSwingMovement->updateNerve();

    if (mSwingMovement->isStop())
        startNerveAction(this, "Stop");
}

void SwingMapParts::exeStop() {
    mSwingMovement->updateNerve();

    if (!mSwingMovement->isStop()) {
        if (mSwingMovement->isLeft())
            startNerveAction(this, "MoveLeft");
        else
            startNerveAction(this, "MoveRight");
    }
}
}  // namespace al
