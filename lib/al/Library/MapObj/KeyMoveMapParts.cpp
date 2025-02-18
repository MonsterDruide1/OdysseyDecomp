#include "Library/MapObj/KeyMoveMapParts.h"

#include "Library/Area/SwitchKeepOnAreaGroup.h"
#include "Library/Area/SwitchOnAreaGroup.h"
#include "Library/Demo/DemoFunction.h"
#include "Library/Fluid/RippleCtrl.h"
#include "Library/KeyPose/KeyPoseKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
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
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

namespace al {
namespace {
NERVE_ACTION_IMPL(KeyMoveMapParts, StandBy)
NERVE_ACTION_IMPL(KeyMoveMapParts, Delay)
NERVE_ACTION_IMPL(KeyMoveMapParts, Wait)
NERVE_ACTION_IMPL(KeyMoveMapParts, MoveSign)
NERVE_ACTION_IMPL(KeyMoveMapParts, Move)
NERVE_ACTION_IMPL(KeyMoveMapParts, StopSign)
NERVE_ACTION_IMPL(KeyMoveMapParts, Stop)

NERVE_ACTIONS_MAKE_STRUCT(KeyMoveMapParts, StandBy, Delay, Wait, MoveSign, Move, StopSign, Stop)
}  // namespace

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

    setKeyMoveClippingInfo(this, &mClippingOffset, mKeyPoseKeeper);

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
    if (mRippleCtrl != nullptr)
        mRippleCtrl->init(info);
}

void KeyMoveMapParts::start() {
    if (!isNerve(this, NrvKeyMoveMapParts.StandBy.data()) || getKeyPoseCount(mKeyPoseKeeper) <= 1)
        return;

    if (isExistAction(this, "Start"))
        startAction(this, "Start");

    if (mDelayTime >= 1)
        startNerveAction(this, "Delay");
    else
        startNerveAction(this, "Wait");
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
        startNerveAction(this, "Delay");
    else
        startNerveAction(this, "Wait");

    if (getKeyPoseCount(mKeyPoseKeeper) < 2 || mIsFloorTouchStart || mIsHipDropStart ||
        isValidStageSwitch(this, "SwitchStart"))
        startNerveAction(this, "StandBy");

    makeActorAlive();
}

bool KeyMoveMapParts::receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) {
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

void KeyMoveMapParts::appearAndSetEnd() {
    resetKeyPose(mKeyPoseKeeper);
    while (!isLastKey(mKeyPoseKeeper))
        nextKeyPose(mKeyPoseKeeper);

    setQuat(this, getCurrentKeyQuat(mKeyPoseKeeper));
    setTrans(this, getCurrentKeyTrans(mKeyPoseKeeper));
    resetPosition(this);

    startNerveAction(this, "Stop");

    if (getKeyPoseCount(mKeyPoseKeeper) < 2 || mIsFloorTouchStart || mIsHipDropStart ||
        isValidStageSwitch(this, "SwitchStart"))
        startNerveAction(this, "StandBy");

    makeActorAlive();
}

void KeyMoveMapParts::exeStandBy() {}

void KeyMoveMapParts::exeDelay() {
    if (isGreaterEqualStep(this, mDelayTime - 1))
        startNerveAction(this, "Wait");
}

void KeyMoveMapParts::exeWait() {
    if (isFirstStep(this)) {
        s32 moveWaitTime = calcKeyMoveWaitTime(mKeyPoseKeeper);
        if (moveWaitTime > -1)
            mKeyMoveWaitTime = moveWaitTime;
    }

    if (isGreaterEqualStep(this, mKeyMoveWaitTime))
        setWaitEndNerve();
}

void KeyMoveMapParts::setWaitEndNerve() {
    if (isRestart(mKeyPoseKeeper)) {
        restartKeyPose(mKeyPoseKeeper, getTransPtr(this), getQuatPtr(this));
        resetPosition(this);
        startNerveAction(this, "Wait");

        return;
    }

    if (isMoveSignKey(mKeyPoseKeeper) && isExistAction(this, "MoveKeySign")) {
        startNerveAction(this, "MoveSign");

        return;
    }

    startNerveAction(this, "Move");
}

void KeyMoveMapParts::exeMoveSign() {
    if (isFirstStep(this))
        startAction(this, "MoveKeySign");

    if (isActionEnd(this))
        startNerveAction(this, "Move");
}

void KeyMoveMapParts::exeMove() {
    if (isFirstStep(this)) {
        if (isExistAction(this, "MoveLoop"))
            tryStartActionIfNotPlaying(this, "MoveLoop");

        mKeyMoveMoveTime = calcKeyMoveMoveTime(mKeyPoseKeeper);

        mSeMoveName = getSeNameByIndex(mKeyPoseKeeper->getKeyPoseCurrentIdx());
        if (mSeMoveName != nullptr)
            tryStartSe(this, mSeMoveName);
    }

    f32 rate = calcNerveRate(this, mKeyMoveMoveTime);
    calcLerpKeyTrans(getTransPtr(this), mKeyPoseKeeper, rate);
    calcSlerpKeyQuat(getQuatPtr(this), mKeyPoseKeeper, rate);

    if (isGreaterEqualStep(this, mKeyMoveMoveTime)) {
        nextKeyPose(mKeyPoseKeeper);
        if (isStop(mKeyPoseKeeper))
            stop();
        else if (calcKeyMoveWaitTime(mKeyPoseKeeper) == 0)
            setWaitEndNerve();
        else {
            startNerveAction(this, "Wait");
            if (mSeMoveName != nullptr) {
                tryStopSe(this, mSeMoveName, -1, nullptr);
                mSeMoveName = nullptr;
            }

            tryStartSe(this, "MoveEnd");
        }
    }
}

void KeyMoveMapParts::exeStopSign() {
    if (isFirstStep(this))
        startAction(this, "StopSign");

    if (isActionEnd(this))
        startNerveAction(this, "Stop");
}

void KeyMoveMapParts::exeStop() {
    if (isFirstStep(this)) {
        if (isExistAction(this, "Stop"))
            tryStartAction(this, "Stop");

        if (isInvalidClipping(this))
            validateClipping(this);

        if (mIsStopKill)
            kill();
    }
}
}  // namespace al
