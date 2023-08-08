#include "al/input/PlayerInput.h"

#include "al/LiveActor/LiveActor.h"
#include "al/input/JoyPadAccelPoseAnalyzer.h"
#include "al/input/PlayerInputFunction.h"
#include "al/input/rsSeparatePlay.h"
#include "al/player/PlayerFunction.h"
#include "al/util/InputUtil.h"
#include "al/util/MathUtil.h"
#include "al/util/VectorUtil.h"
#include "rs/Dimension.h"

PlayerInput::PlayerInput(const al::LiveActor*, const IUsePlayerCollision*, const IUseDimension*) {
}  // FIXME remove this

bool PlayerInput::isEnableCarry() const {
    if (mDisableInput)
        return false;

    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerCarryStart() const {
    if (mDisableInput)
        return false;

    return PlayerInputFunction::isTriggerAction(mLiveActor,
                                                PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerCarryRelease() const {
    if (mDisableInput)
        return false;

    if (PlayerInputFunction::isTriggerAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor)))
        return true;

    if (mDisableInput)
        return false;

    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerSwingActionMario() const {
    if (mDisableInput)
        return false;

    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerCarryReleaseBySwing() const {
    if (mDisableInput)
        return false;
    if (!PlayerInputFunction::isTriggerAction(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor))) {
        if (mDisableInput)
            return false;
        if (!mJoyPadAccelPoseAnalyzer1->isSwingAnyHand())
            return false;
    }
    if (mDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerAction() const {
    if (mDisableInput)
        return false;
    return PlayerInputFunction::isTriggerAction(mLiveActor,
                                                PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerJump() const {
    if (mDisableInput)
        return false;
    auto* dimension = mDimension;
    if (dimension && rs::is2D(dimension) && rs::isIn2DArea(dimension)) {
        if (!mDisableInput && mJoyPadAccelPoseAnalyzer1->isSwingAnyHand())
            return true;
    }
    return PlayerInputFunction::isTriggerJump(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerHipDrop() const {
    if (mDisableInput)
        return false;
    return PlayerInputFunction::isTriggerSubAction(mLiveActor,
                                                   PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerHeadSliding() const {
    if (mDisableInput)
        return false;
    if (PlayerInputFunction::isTriggerAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor)))
        return true;
    if (mDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerPaddle() const {
    if (mDisableInput)
        return false;
    return PlayerInputFunction::isTriggerJump(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerRolling(bool a1) const {
    if (mDisableInput)
        return false;
    if (!PlayerInputFunction::isHoldSubAction(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor)) &&
        !a1)
        return false;
    if (PlayerInputFunction::isTriggerAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor)))
        return true;
    if (mDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerRollingRestartSwing() const {
    if (mDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerRollingCancelHipDrop(bool a1) const {
    if (mDisableInput)
        return false;
    if (!PlayerInputFunction::isHoldSubAction(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor)) &&
        !a1)
        return false;
    if (PlayerInputFunction::isTriggerAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor)))
        return true;
    if (mDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerHackAction() const {
    if (mDisableInput)
        return false;
    return PlayerInputFunction::isTriggerAction(mLiveActor,
                                                PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerHackJump() const {
    if (mDisableInput)
        return false;
    return PlayerInputFunction::isTriggerJump(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerHackSwing() const {
    if (mDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerHackEnd() const {
    if (mDisableInput)
        return false;
    auto inputPort = PlayerFunction::getPlayerInputPort(mLiveActor);
    return PlayerInputFunction::isTriggerSubAction(mLiveActor, inputPort);
}

bool PlayerInput::isTriggerHackSeparateJump() const {
    if (mDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    return PlayerInputFunction::isTriggerJump(mLiveActor, inputPort);
}

bool PlayerInput::isTriggerSeparateCapJangoHelp() const {
    if (mDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    return PlayerInputFunction::isTriggerAction(mLiveActor, inputPort) ||
           PlayerInputFunction::isTriggerJump(mLiveActor, inputPort) ||
           PlayerInputFunction::isTriggerSubAction(mLiveActor, inputPort);
}

bool PlayerInput::isHoldHackSeparateJump() const {
    if (mDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    return PlayerInputFunction::isHoldJump(mLiveActor, inputPort);
}

bool PlayerInput::isTriggerGetOff() const {
    if (mDisableInput)
        return false;
    return PlayerInputFunction::isTriggerSubAction(mLiveActor,
                                                   PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldAction() const {
    if (mDisableInput)
        return false;
    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldJump() const {
    if (mDisableInput)
        return false;
    auto* dimension = mDimension;
    if (dimension && rs::is2D(dimension) && rs::isIn2DArea(dimension) && _88 > 0) {
        return true;
    }
    return PlayerInputFunction::isHoldJump(mLiveActor,
                                           PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldHipDrop() const {
    if (mDisableInput)
        return false;
    return PlayerInputFunction::isHoldSubAction(mLiveActor,
                                                PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerStartTalk() const {
    if (mDisableInput)
        return false;
    return PlayerInputFunction::isTriggerTalk(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerStartWorldWarp() const {
    if (mDisableInput)
        return false;
    return PlayerInputFunction::isTriggerStartWorldWarp(
        mLiveActor, PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerCancelWorldWarp() const {
    if (mDisableInput)
        return false;
    return PlayerInputFunction::isTriggerCancelWorldWarp(
        mLiveActor, PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerSpinCap() const {
    if (mDisableInput)
        return false;
    auto inputPort = PlayerFunction::getPlayerInputPort(mLiveActor);
    if (PlayerInputFunction::isTriggerAction(mLiveActor, inputPort))
        return true;
    if (mDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerToggleStayCap() const {
    if (mDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    return PlayerInputFunction::isTriggerAction(mLiveActor, inputPort);
}

bool PlayerInput::isTriggerSpinAttackSeparate() const {
    if (mDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor))
        return false;
    if (!mDisableInput && mJoyPadAccelPoseAnalyzer1->isSwingAnyHand())
        return true;
    return PlayerInputFunction::isTriggerAction(mLiveActor, al::getPlayerControllerPort(0));
}

int PlayerInput::getSeparatePlay1P() {
    return al::getPlayerControllerPort(0);
}

bool PlayerInput::isTriggerCapReturn() const {
    if (mDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor)) {
        if (mDisableInput)
            return false;
        auto inputPort = PlayerFunction::getPlayerInputPort(mLiveActor);
        if (PlayerInputFunction::isTriggerAction(mLiveActor, inputPort))
            return true;
        if (mDisableInput)
            return false;
        return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
    }
    if (mDisableInput || !rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    if (PlayerInputFunction::isTriggerAction(mLiveActor, inputPort))
        return true;
    if (mDisableInput || !rs::isSeparatePlay(mLiveActor))
        return false;
    return mJoyPadAccelPoseAnalyzer2->isSwingAnyHand();
}

bool PlayerInput::isTriggerCapAttackSeparate() const {
    if (mDisableInput || !rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    if (PlayerInputFunction::isTriggerAction(mLiveActor, inputPort))
        return true;
    if (mDisableInput || !rs::isSeparatePlay(mLiveActor))
        return false;
    return mJoyPadAccelPoseAnalyzer2->isSwingAnyHand();
}

bool PlayerInput::isTriggerSwingActionCap() const {
    if (mDisableInput || !rs::isSeparatePlay(mLiveActor))
        return false;
    return mJoyPadAccelPoseAnalyzer2->isSwingAnyHand();
}

bool PlayerInput::isTriggerCapSingleHandThrow() const {
    if (mDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isTriggerCapDoubleHandThrow() const {
    if (mDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingDoubleHandSameDir();
}

bool PlayerInput::isTriggerCapSeparateJump() const {
    if (mDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    return PlayerInputFunction::isTriggerJump(mLiveActor, inputPort);
}

bool PlayerInput::isTriggerCapSeparateHipDrop() const {
    if (mDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor))
        return false;
    auto inputPort = al::getPlayerControllerPort(1);
    return PlayerInputFunction::isTriggerSubAction(mLiveActor, inputPort);
}

bool PlayerInput::isTriggerSwingPoleClimbFast() const {
    if (mDisableInput)
        return false;
    return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
}

bool PlayerInput::isHoldPoleClimbDown() const {
    if (mDisableInput)
        return false;
    return PlayerInputFunction::isHoldSubAction(mLiveActor,
                                                PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerAppendCapAttack(bool a1) const {
    if (mDisableInput)
        return false;
    if (!rs::isSeparatePlay(mLiveActor) || a1) {
        if (mDisableInput)
            return false;
        return mJoyPadAccelPoseAnalyzer1->isSwingAnyHand();
    }
    if (mDisableInput || !rs::isSeparatePlay(mLiveActor))
        return false;
    return mJoyPadAccelPoseAnalyzer2->isSwingAnyHand();
}

bool PlayerInput::isHoldSpinCap() const {
    if (mDisableInput || rs::isSeparatePlay(mLiveActor))
        return false;
    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldCapAction() const {
    if (mDisableInput)
        return false;
    if (rs::isSeparatePlay(mLiveActor))
        return !PlayerInputFunction::isTriggerAction(mLiveActor, al::getPlayerControllerPort(1));
    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldPoleClimbFast() const {
    if (mDisableInput)
        return false;
    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldWallCatchMoveFast() const {
    if (mDisableInput)
        return false;
    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldHackAction() const {
    if (mDisableInput)
        return false;
    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isHoldHackJump() const {
    if (mDisableInput)
        return false;
    return PlayerInputFunction::isHoldJump(mLiveActor,
                                           PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isTriggerChange2D() const {
    if (mDisableInput)
        return false;
    auto inputPort = PlayerFunction::getPlayerInputPort(mLiveActor);
    return al::isPadTriggerZL(inputPort) || al::isPadTriggerZR(inputPort);
}

bool PlayerInput::isTriggerChange3D() const {
    if (mDisableInput)
        return false;
    auto inputPort = PlayerFunction::getPlayerInputPort(mLiveActor);
    return al::isPadTriggerZL(inputPort) || al::isPadTriggerZR(inputPort);
}

bool PlayerInput::isReleaseJump() const {
    if (mDisableInput)
        return false;
    return PlayerInputFunction::isReleaseJump(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isReleaseHackAction() const {
    if (mDisableInput)
        return false;
    return PlayerInputFunction::isReleaseAction(mLiveActor,
                                                PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isReleaseHackJump() const {
    if (mDisableInput)
        return false;
    return PlayerInputFunction::isReleaseJump(mLiveActor,
                                              PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isEnableDashInput() const {
    if (mDisableInput)
        return false;
    return PlayerInputFunction::isHoldAction(mLiveActor,
                                             PlayerFunction::getPlayerInputPort(mLiveActor));
}

bool PlayerInput::isThrowTypeSpiral(const sead::Vector2f& a1) const {
    if (al::isNearZero(a1, 0.001))
        return false;
    float absX = (a1.x > 0 ? a1.x : -a1.x);
    float absY = (a1.y > 0 ? a1.y : -a1.y);
    return absX > absY;
}

bool PlayerInput::isThrowTypeRolling(const sead::Vector2f& a1) const {
    if (al::isNearZero(a1, 0.001))
        return !al::isNearZero(a1.y, 0.001);
    float absX = (a1.x > 0 ? a1.x : -a1.x);
    float absY = (a1.y > 0 ? a1.y : -a1.y);
    return !(absX > absY) && !al::isNearZero(a1.y, 0.001);
}
