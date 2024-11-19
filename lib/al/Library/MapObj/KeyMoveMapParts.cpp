#include "Library/MapObj/KeyMoveMapParts.h"

#include "Library/Demo/DemoFunction.h"
#include "Library/Fluid/RippleCtrl.h"
#include "Library/KeyPose/KeyPoseKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/MapObj/ChildStep.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Se/SeFunction.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

namespace {
using namespace al;

NERVE_ACTION_IMPL(KeyMoveMapParts, StandBy)
NERVE_ACTION_IMPL(KeyMoveMapParts, Delay)
NERVE_ACTION_IMPL(KeyMoveMapParts, Wait)
NERVE_ACTION_IMPL(KeyMoveMapParts, MoveSign)
NERVE_ACTION_IMPL(KeyMoveMapParts, Move)
NERVE_ACTION_IMPL(KeyMoveMapParts, StopSign)
NERVE_ACTION_IMPL(KeyMoveMapParts, Stop)

NERVE_ACTIONS_MAKE_STRUCT(KeyMoveMapParts, StandBy, Delay, Wait, MoveSign, Move, StopSign, Stop)
}  // namespace

namespace al {
KeyMoveMapParts::KeyMoveMapParts(const char* name) : LiveActor(name) {}

void KeyMoveMapParts::init(const ActorInitInfo& info) {
    using KeyMoveMapPartsFunctor = FunctorV0M<KeyMoveMapParts*, void (KeyMoveMapParts::*)()>;

    initNerveAction(this, "Wait", &NrvKeyMoveMapParts.mCollector, 0);

    const char* suffix = nullptr;
    tryGetStringArg(&suffix, info, "SuffixName");
    initMapPartsActor(this, info, suffix);

    tryGetQuatPtr(this);

    mKeyPoseKeeper = createKeyPoseKeeper(info);

    registerAreaHostMtx(this, info);

    setKeyMoveClippingInfo(this, &_128, mKeyPoseKeeper);

    tryGetArg(&mIsFloorTouchStart, info, "IsFloorTouchStart");
    tryGetArg(&mIsHipDropStart, info, "IsHipDropStart");
    tryGetArg(&mIsStopKill, info, "IsStopKill");
    tryGetArg(&mDelayTime, info, "DelayTime");

    if (mDelayTime > 0)
        startNerveAction(this, "Delay");

    bool isListenStart =
        listenStageSwitchOnStart(this, KeyMoveMapPartsFunctor(this, &KeyMoveMapParts::start));

    if (getKeyPoseCount(mKeyPoseKeeper) < 2 || mIsFloorTouchStart || mIsHipDropStart ||
        isListenStart)
        startNerveAction(this, "StandBy");

    mSwitchKeepOnAreaGroup = tryCreateSwitchKeepOnAreaGroup(this, info);
    mSwitchOnAreaGroup = tryCreateSwitchOnAreaGroup(this, info);

    trySyncStageSwitchAppear(this);
    tryListenStageSwitchKill(this);
    listenStageSwitchOn(this, "SwitchStop", KeyMoveMapPartsFunctor(this, &KeyMoveMapParts::stop));
    listenStageSwitchOn(this, "SwitchReset",
                        KeyMoveMapPartsFunctor(this, &KeyMoveMapParts::appearAndSetStart));

    registActorToDemoInfo(this, info);

    tryInitSubActorKeeperChildStep(this, info);
    createChildStep(info, this, true);
    if (!isExistModel(this))
        return;

    mRippleCtrl = RippleCtrl::tryCreate(this);
    if (mRippleCtrl == nullptr)
        return;

    mRippleCtrl->init(info);
}

void KeyMoveMapParts::start() {
    if (!isNerve(this, NrvKeyMoveMapParts.StandBy.data()) || getKeyPoseCount(mKeyPoseKeeper) <= 1)
        return;

    if (isExistAction(this, "Start"))
        startAction(this, "Start");

    if (mDelayTime >= 1) {
        startAction(this, "Delay");

        return;
    }

    startAction(this, "Wait");
}

void KeyMoveMapParts::stop() {
    if (isNerve(this, NrvKeyMoveMapParts.StopSign.data()) ||
        isNerve(this, NrvKeyMoveMapParts.Stop.data()))
        return;

    if (isExistAction(this, "StopSign"))
        startNerveAction(this, "StopSign");
    else
        startNerveAction(this, "Stop");

    if (mSeMoveName != nullptr) {
        tryStopSe(this, mSeMoveName, -1, nullptr);
        mSeMoveName = nullptr;
    }

    tryStartSe(this, "MoveEnd");
}

void KeyMoveMapParts::appearAndSetStart() {
    resetKeyPose(mKeyPoseKeeper);
    setQuat(this, getCurrentKeyQuat(mKeyPoseKeeper));
    setTrans(this, getCurrentKeyTrans(mKeyPoseKeeper));
    resetPosition(this);

    if (mDelayTime >= 1)
        startAction(this, "Delay");
    else
        startAction(this, "Wait");

    if (getKeyPoseCount(mKeyPoseKeeper) < 2 || mIsFloorTouchStart || mIsHipDropStart ||
        isValidStageSwitch(this, "SwitchStart"))
        startNerveAction(this, "StandBy");

    makeActorAlive();
}

bool KeyMoveMapParts::receiveMsg(const SensorMsg* message, HitSensor* source, HitSensor* target) {
    if (mIsFloorTouchStart && isMsgFloorTouch(message)) {
        start();

        return true;
    }

    if (mIsHipDropStart && isMsgPlayerHipDropAll(message)) {
        start();

        return false;
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

void KeyMoveMapParts::control() {
    if (mSwitchKeepOnAreaGroup != nullptr)
        mSwitchKeepOnAreaGroup->update(getTrans(this));

    if (mSwitchOnAreaGroup != nullptr)
        mSwitchOnAreaGroup->update(getTrans(this));

    if (mRippleCtrl != nullptr)
        mRippleCtrl->update();
}
}  // namespace al
