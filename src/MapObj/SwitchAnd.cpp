#include "MapObj/SwitchAnd.h"

#include "Library/Demo/DemoFunction.h"
#include "Library/KeyPose/KeyPoseKeeper.h"
#include "Library/KeyPose/KeyPoseKeeperUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Se/SeFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

namespace {
using al::SwitchKeyMoveMapParts;

NERVE_ACTION_IMPL(SwitchKeyMoveMapParts, StandBy)
NERVE_ACTION_IMPL(SwitchKeyMoveMapParts, Wait)
NERVE_ACTION_IMPL(SwitchKeyMoveMapParts, MoveSign)
NERVE_ACTION_IMPL(SwitchKeyMoveMapParts, Move)
NERVE_ACTION_IMPL(SwitchKeyMoveMapParts, StopSign)
NERVE_ACTION_IMPL(SwitchKeyMoveMapParts, Stop)

NERVE_ACTIONS_MAKE_STRUCT(SwitchKeyMoveMapParts, StandBy, Wait, MoveSign, Move, StopSign, Stop)
}  // namespace

SwitchAnd::SwitchAnd(const char* name) : al::LiveActor(name) {}

void SwitchAnd::init(const al::ActorInitInfo& info) {
    using SwitchAndFunctor = al::FunctorV0M<SwitchAnd*, void (SwitchAnd::*)()>;

    al::initActorSceneInfo(this, info);
    al::initStageSwitch(this, info);

    if (al::listenStageSwitchOnOff(this, "InputSwitchA",
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOn),
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOff)))
        mInputSwitchNum++;

    if (al::listenStageSwitchOnOff(this, "InputSwitchB",
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOn),
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOff)))
        mInputSwitchNum++;

    if (al::listenStageSwitchOnOff(this, "InputSwitchC",
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOn),
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOff)))
        mInputSwitchNum++;

    if (al::listenStageSwitchOnOff(this, "InputSwitchD",
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOn),
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOff)))
        mInputSwitchNum++;

    if (al::listenStageSwitchOnOff(this, "InputSwitchE",
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOn),
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOff)))
        mInputSwitchNum++;

    if (al::listenStageSwitchOnOff(this, "InputSwitchF",
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOn),
                                   SwitchAndFunctor(this, &SwitchAnd::notifyInputSwitchOff)))
        mInputSwitchNum++;

    makeActorDead();
}

void SwitchAnd::notifyInputSwitchOn() {
    mInputSwitchNum--;
    if (mInputSwitchNum == 0)
        al::tryOnStageSwitch(this, "OutputSwitchOn");
}

void SwitchAnd::notifyInputSwitchOff() {
    mInputSwitchNum++;
    if (mInputSwitchNum != 0)
        al::tryOffStageSwitch(this, "OutputSwitchOn");
}

namespace al {
SwitchKeyMoveMapParts::SwitchKeyMoveMapParts(const char* name) : LiveActor(name) {}

void SwitchKeyMoveMapParts::init(const ActorInitInfo& info) {
    using SwitchKeyMoveMapPartsFunctor =
        FunctorV0M<SwitchKeyMoveMapParts*, void (SwitchKeyMoveMapParts::*)()>;

    initNerveAction(this, "StandBy", &NrvSwitchKeyMoveMapParts.collector, 0);
    initMapPartsActor(this, info, nullptr);
    tryGetQuatPtr(this);
    mKeyPoseKeeper = createKeyPoseKeeper(info);
    mKeyPoseKeeper->setMoveTypeStop();
    setKeyMoveClippingInfo(this, &mClippingOffset, mKeyPoseKeeper);
    registActorToDemoInfo(this, info);
    listenStageSwitchOnOffStart(this,
                                SwitchKeyMoveMapPartsFunctor(this, &SwitchKeyMoveMapParts::on),
                                SwitchKeyMoveMapPartsFunctor(this, &SwitchKeyMoveMapParts::off));
    makeActorAlive();
}

void SwitchKeyMoveMapParts::on() {
    if (mIsReverse) {
        reverse();
        return;
    }

    if (isNerve(this, NrvSwitchKeyMoveMapParts.StandBy.data())) {
        if (isExistAction(this, "Start"))
            startAction(this, "Start");

        startNerveAction(this, "Wait");
    }
}

void SwitchKeyMoveMapParts::off() {
    if (!mIsReverse) {
        reverse();
        return;
    }

    if (isNerve(this, NrvSwitchKeyMoveMapParts.Stop.data())) {
        if (isExistAction(this, "Start"))
            startAction(this, "Start");

        startNerveAction(this, "Wait");
    }
}

void SwitchKeyMoveMapParts::reverse() {
    if (isNerve(this, NrvSwitchKeyMoveMapParts.Wait.data()) ||
        isNerve(this, NrvSwitchKeyMoveMapParts.MoveSign.data())) {
        startNerveAction(this, "Stop");
    } else if (isNerve(this, NrvSwitchKeyMoveMapParts.Move.data())) {
        nextKeyPose(mKeyPoseKeeper);
        reverseKeyPose(mKeyPoseKeeper);

        mMoveRate = 1.0f - sead::Mathf::clamp(
                               (f32)getNerveStep(this) / mKeyMoveMoveTime + mMoveRate, 0.0f, 1.0f);

        startNerveAction(this, "Move");
    } else if (isNerve(this, NrvSwitchKeyMoveMapParts.StopSign.data()) ||
               isNerve(this, NrvSwitchKeyMoveMapParts.Stop.data()) ||
               isNerve(this, NrvSwitchKeyMoveMapParts.StandBy.data())) {
        reverseKeyPose(mKeyPoseKeeper);
        if (isExistAction(this, "Start"))
            startAction(this, "Start");

        startNerveAction(this, "Wait");
    }

    mIsReverse = !mIsReverse;
}

void SwitchKeyMoveMapParts::stop() {
    if (isNerve(this, NrvSwitchKeyMoveMapParts.StopSign.data()) ||
        isNerve(this, NrvSwitchKeyMoveMapParts.Stop.data()))
        return;

    if (isExistAction(this, "StopSign"))
        startNerveAction(this, "StopSign");
    else
        startNerveAction(this, "Stop");

    tryStartSe(this, "MoveEnd");
}

void SwitchKeyMoveMapParts::exeStandBy() {}

void SwitchKeyMoveMapParts::exeWait() {
    if (isFirstStep(this)) {
        s32 keyMoveWaitTime = calcKeyMoveWaitTime(mKeyPoseKeeper);
        if (keyMoveWaitTime >= 0)
            mKeyMoveWaitTime = keyMoveWaitTime;
    }

    if (isGreaterEqualStep(this, mKeyMoveWaitTime)) {
        if (isMoveSignKey(mKeyPoseKeeper) && isExistAction(this, "MoveKeySign")) {
            startNerveAction(this, "MoveSign");
        } else {
            mKeyMoveMoveTime = calcKeyMoveMoveTime(mKeyPoseKeeper);
            startNerveAction(this, "Move");
        }
    }
}

void SwitchKeyMoveMapParts::exeMoveSign() {
    if (isFirstStep(this))
        startAction(this, "MoveKeySign");

    if (isActionEnd(this)) {
        mKeyMoveMoveTime = calcKeyMoveMoveTime(mKeyPoseKeeper);
        startNerveAction(this, "Move");
    }
}

void SwitchKeyMoveMapParts::exeMove() {
    if (isFirstStep(this) && isExistAction(this, "MoveLoop"))
        tryStartActionIfNotPlaying(this, "MoveLoop");

    f32 rate =
        sead::Mathf::clamp((f32)getNerveStep(this) / mKeyMoveMoveTime + mMoveRate, 0.0f, 1.0f);

    calcLerpKeyTrans(getTransPtr(this), mKeyPoseKeeper, rate);
    calcSlerpKeyQuat(getQuatPtr(this), mKeyPoseKeeper, rate);

    if (rate >= 1.0f) {
        KeyPoseKeeper* keyPoseKeeper = mKeyPoseKeeper;
        mMoveRate = 0.0f;
        nextKeyPose(keyPoseKeeper);

        if (!isStop(mKeyPoseKeeper)) {
            startNerveAction(this, "Wait");
            tryStartSe(this, "MoveEnd");
        } else if (!isNerve(this, NrvSwitchKeyMoveMapParts.StopSign.data()) &&
                   !isNerve(this, NrvSwitchKeyMoveMapParts.Stop.data())) {
            if (isExistAction(this, "StopSign"))
                startNerveAction(this, "StopSign");
            else
                startNerveAction(this, "Stop");

            tryStartSe(this, "MoveEnd");
        }
    }
}

void SwitchKeyMoveMapParts::exeStopSign() {
    if (isFirstStep(this))
        startAction(this, "StopSign");

    if (isActionEnd(this))
        startNerveAction(this, "Stop");
}

void SwitchKeyMoveMapParts::exeStop() {
    if (isFirstStep(this) && isExistAction(this, "Stop"))
        tryStartAction(this, "Stop");
}
}  // namespace al
