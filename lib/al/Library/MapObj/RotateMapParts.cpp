#include "Library/MapObj/RotateMapParts.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/MapObj/ChildStep.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Se/SeFunction.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Thread/FunctorV0M.h"

namespace al {
namespace {
NERVE_ACTION_IMPL(RotateMapParts, StandBy)
NERVE_ACTION_IMPL(RotateMapParts, Rotate)
NERVE_ACTION_IMPL(RotateMapParts, AssistStop)

NERVE_ACTIONS_MAKE_STRUCT(RotateMapParts, StandBy, Rotate, AssistStop)
}  // namespace

RotateMapParts::RotateMapParts(const char* name) : LiveActor(name) {}

void RotateMapParts::init(const ActorInitInfo& info) {
    using RotateMapPartsFunctor = FunctorV0M<RotateMapParts*, void (RotateMapParts::*)()>;

    tryInitSubActorKeeperChildStep(this, info);
    initNerveAction(this, "Rotate", &NrvRotateMapParts.mCollector, 0);
    initMapPartsActor(this, info, nullptr);
    tryGetQuatPtr(this);
    registerAreaHostMtx(this, info);

    mQuat.set(getQuat(this));
    tryGetArg((s32*)&mRotateAxis, info, "RotateAxis");
    tryGetArg(&mRotateSpeed, info, "RotateSpeed");

    createChildStep(info, this, true);
    initMaterialCode(this, info);

    if (listenStageSwitchOnStart(this, RotateMapPartsFunctor(this, &RotateMapParts::start)))
        startNerveAction(this, "StandBy");

    trySyncStageSwitchAppear(this);
}

void RotateMapParts::start() {
    if (isNerve(this, NrvRotateMapParts.StandBy.data()))
        startNerveAction(this, "Rotate");
}

bool RotateMapParts::receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) {
    if (isMsgTouchAssist(message)) {
        mAssistTimer = 45;

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

    if (isMsgRestart(message)) {
        appearAndSetStart();

        return true;
    }

    return false;
}

void RotateMapParts::appearAndSetStart() {
    mAssistTimer = 0;
    setQuat(this, mQuat);

    makeActorAlive();
}

void RotateMapParts::exeStandBy() {}

void RotateMapParts::exeRotate() {
    rotateQuatLocalDirDegree(this, (s32)mRotateAxis, mRotateSpeed / 100.0f);
    if (mAssistTimer > 0)
        startNerveAction(this, "AssistStop");

    tryHoldSeWithParam(this, "RotateWithSpeed", mRotateSpeed, "");
}

void RotateMapParts::exeAssistStop() {
    mAssistTimer--;
    if (mAssistTimer <= 0) {
        mAssistTimer = 0;
        startNerveAction(this, "Rotate");
    }
}
}  // namespace al
