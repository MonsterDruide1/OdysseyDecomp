#include "Player/PlayerInput.h"

#include "Library/Controller/InputFunction.h"
#include "Library/Controller/JoyPadAccelPoseAnalyzer.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/VectorUtil.h"

#include "Player/PlayerFunction.h"
#include "Player/PlayerInputFunction.h"
#include "Util/ActorDimensionKeeper.h"

bool PlayerInput::isEnableCarry() const {
    if (mIsDisableInput)
        return false;

    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerCarryStart() const {
    if (mIsDisableInput)
        return false;

    return PlayerInputFunction::isTriggerAction(mLiveActor,
                                                PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerCarryRelease() const {
    if (mIsDisableInput)
        return false;

    if (PlayerInputFunction::isTriggerAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor)))
        return true;

    if (mIsDisableInput)
        return false;

    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerSwingActionMario() const {
    if (mIsDisableInput)
        return false;

    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerCarryReleaseBySwing() const {
    if (mIsDisableInput)
        return false;
    if (!PlayerInputFunction::isTriggerAction(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor))) {
        if (mIsDisableInput)
            return false;
        if (!mJoyPadAccelPoseAnalyzer1->isSwingAnyHand())
            return false;
    }
    if (mIsDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerAction() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isTriggerAction(mLiveActor,
                                                PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerJump() const {
    if (mIsDisableInput)
        return false;
    auto* dimension = mDimension;
    if (dimension && rs::is2D(dimension) && rs::isIn2DArea(dimension)) {
        if (!mIsDisableInput && mJoyPadAccelPoseAnalyzer1->isSwingAnyHand())
            return true;
    }
    return PlayerInputFunction::isTriggerJump(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerHipDrop() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isTriggerSubAction(mLiveActor,
                                                   PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerHeadSliding() const {
    if (mIsDisableInput)
        return false;
    if (PlayerInputFunction::isTriggerAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor)))
        return true;
    if (mIsDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerPaddle() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isTriggerJump(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerRolling(bool a1) const {
    if (mIsDisableInput)
        return false;
    if (!PlayerInputFunction::isHoldSubAction(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor)) &&
        !a1)
        return false;
    if (PlayerInputFunction::isTriggerAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor)))
        return true;
    if (mIsDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerRollingRestartSwing() const {
    if (mIsDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerRollingCancelHipDrop(bool a1) const {
    if (mIsDisableInput)
        return false;
    if (!PlayerInputFunction::isHoldSubAction(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor)) &&
        !a1)
        return false;
    if (PlayerInputFunction::isTriggerAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor)))
        return true;
    if (mIsDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerHackAction() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isTriggerAction(mLiveActor,
                                                PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerHackJump() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isTriggerJump(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerHackSwing() const {
    if (mIsDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerHackEnd() const {
    if (mIsDisableInput)
        return false;
    auto inputPort = PlayerFunction::getPlayerInputPort(mLiveActor);
    return PlayerInputFunction::isTriggerSubAction(mLiveActor, inputPort);
}

bool PlayerInput::isTriggerHackSeparateJump() const {
    if (mIsDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    return PlayerInputFunction::isTriggerJump(mLiveActor, inputPort);
}

bool PlayerInput::isTriggerSeparateCapJangoHelp() const {
    if (mIsDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    return PlayerInputFunction::isTriggerAction(mLiveActor, inputPort) ||
           PlayerInputFunction::isTriggerJump(mLiveActor, inputPort) ||
           PlayerInputFunction::isTriggerSubAction(mLiveActor, inputPort);
}

bool PlayerInput::isHoldHackSeparateJump() const {
    if (mIsDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    return PlayerInputFunction::isHoldJump(mLiveActor, inputPort);
}

bool PlayerInput::isTriggerGetOff() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isTriggerSubAction(mLiveActor,
                                                   PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldAction() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldJump() const {
    if (mIsDisableInput)
        return false;
    auto* dimension = mDimension;
    if (dimension && rs::is2D(dimension) && rs::isIn2DArea(dimension) && _88 > 0)
        return true;
    return PlayerInputFunction::isHoldJump(mLiveActor,
                                           PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldHipDrop() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isHoldSubAction(mLiveActor,
                                                PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerStartTalk() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isTriggerTalk(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerStartWorldWarp() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isTriggerStartWorldWarp(
        mLiveActor, PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerCancelWorldWarp() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isTriggerCancelWorldWarp(
        mLiveActor, PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerSpinCap() const {
    if (mIsDisableInput)
        return false;
    auto inputPort = PlayerFunction::getPlayerInputPort(mLiveActor);
    if (PlayerInputFunction::isTriggerAction(mLiveActor, inputPort))
        return true;
    if (mIsDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerToggleStayCap() const {
    if (mIsDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    return PlayerInputFunction::isTriggerAction(mLiveActor, inputPort);
}

bool PlayerInput::isTriggerSpinAttackSeparate() const {
    if (mIsDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor))
        return false;
    if (!mIsDisableInput && mJoyPadAccelPoseAnalyzer1->isSwingAnyHand())
        return true;
    return PlayerInputFunction::isTriggerAction(mLiveActor, al::getPlayerControllerPort(0));
}

s32 PlayerInput::getSeparatePlay1P() {
    return al::getPlayerControllerPort(0);
}

bool PlayerInput::isTriggerCapReturn() const {
    if (mIsDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor)) {
        if (mIsDisableInput)
            return false;
        auto inputPort = PlayerFunction::getPlayerInputPort(mLiveActor);
        if (PlayerInputFunction::isTriggerAction(mLiveActor, inputPort))
            return true;
        if (mIsDisableInput)
            return false;
        return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
    }
    if (mIsDisableInput || !rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    if (PlayerInputFunction::isTriggerAction(mLiveActor, inputPort))
        return true;
    if (mIsDisableInput || !rs::isSeparatePlay(mLiveActor))
        return false;
    return mJoyPadAccelPoseAnalyzer2->isSwingAnyHand();
}

bool PlayerInput::isTriggerCapAttackSeparate() const {
    if (mIsDisableInput || !rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    if (PlayerInputFunction::isTriggerAction(mLiveActor, inputPort))
        return true;
    if (mIsDisableInput || !rs::isSeparatePlay(mLiveActor))
        return false;
    return mJoyPadAccelPoseAnalyzer2->isSwingAnyHand();
}

bool PlayerInput::isTriggerSwingActionCap() const {
    if (mIsDisableInput || !rs::isSeparatePlay(mLiveActor))
        return false;
    return mJoyPadAccelPoseAnalyzer2->isSwingAnyHand();
}

bool PlayerInput::isTriggerCapSingleHandThrow() const {
    if (mIsDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerCapDoubleHandThrow() const {
    if (mIsDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingDoubleHandSameDir();
}

bool PlayerInput::isTriggerCapSeparateJump() const {
    if (mIsDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    return PlayerInputFunction::isTriggerJump(mLiveActor, inputPort);
}

bool PlayerInput::isTriggerCapSeparateHipDrop() const {
    if (mIsDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    return PlayerInputFunction::isTriggerSubAction(mLiveActor, inputPort);
}

bool PlayerInput::isTriggerSwingPoleClimbFast() const {
    if (mIsDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isHoldPoleClimbDown() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isHoldSubAction(mLiveActor,
                                                PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerAppendCapAttack(bool a1) const {
    if (mIsDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor) || a1) {
        if (mIsDisableInput)
            return false;
        return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
    }
    if (mIsDisableInput || !rs::isSeparatePlay(mLiveActor))
        return false;
    return mJoyPadAccelPoseAnalyzer2->isSwingAnyHand();
}

bool PlayerInput::isHoldSpinCap() const {
    if (mIsDisableInput || rs::isSeparatePlay(mLiveActor))
        return false;
    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldCapAction() const {
    if (mIsDisableInput)
        return false;
    if (rs::isSeparatePlay(mLiveActor))
        return !PlayerInputFunction::isTriggerAction(mLiveActor, al::getPlayerControllerPort(1));
    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldPoleClimbFast() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldWallCatchMoveFast() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldHackAction() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldHackJump() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isHoldJump(mLiveActor,
                                           PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerChange2D() const {
    if (mIsDisableInput)
        return false;
    auto inputPort = PlayerFunction::getPlayerInputPort(mLiveActor);
    return al::isPadTriggerZL(inputPort) || al::isPadTriggerZR(inputPort);
}

bool PlayerInput::isTriggerChange3D() const {
    if (mIsDisableInput)
        return false;
    auto inputPort = PlayerFunction::getPlayerInputPort(mLiveActor);
    return al::isPadTriggerZL(inputPort) || al::isPadTriggerZR(inputPort);
}

bool PlayerInput::isReleaseJump() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isReleaseJump(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isReleaseHackAction() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isReleaseAction(mLiveActor,
                                                PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isReleaseHackJump() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isReleaseJump(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isEnableDashInput() const {
    if (mIsDisableInput)
        return false;
    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isThrowTypeSpiral(const sead::Vector2f& a1) const {
    if (al::isNearZero(a1, 0.001))
        return false;
    f32 absX = (a1.x > 0 ? a1.x : -a1.x);
    f32 absY = (a1.y > 0 ? a1.y : -a1.y);
    return absX > absY;
}

bool PlayerInput::isThrowTypeRolling(const sead::Vector2f& a1) const {
    if (al::isNearZero(a1, 0.001))
        return !al::isNearZero(a1.y, 0.001);
    f32 absX = (a1.x > 0 ? a1.x : -a1.x);
    f32 absY = (a1.y > 0 ? a1.y : -a1.y);
    return !(absX > absY) && !al::isNearZero(a1.y, 0.001);
}
