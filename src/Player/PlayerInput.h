#pragma once

#include <math/seadVector.h>

#include "Player/IUsePlayerCollision.h"
#include "Util/IUseDimension.h"

namespace al {
class LiveActor;
class SpinInputAnalyzer;
class JoyPadAccelPoseAnalyzer;
}  // namespace al

class PlayerInput {
public:
    PlayerInput(const al::LiveActor*, const IUsePlayerCollision*, const IUseDimension*);

    bool isEnableCarry() const;
    bool isTriggerCarryStart() const;
    bool isTriggerCarryRelease() const;
    bool isTriggerSwingActionMario() const;
    bool isTriggerCarryReleaseBySwing() const;
    bool isTriggerAction() const;
    bool isTriggerJump() const;
    bool isTriggerHipDrop() const;
    bool isTriggerHeadSliding() const;
    bool isTriggerPaddle() const;
    bool isTriggerRolling(bool) const;
    bool isTriggerRollingRestartSwing() const;
    bool isTriggerRollingCancelHipDrop(bool) const;
    bool isTriggerHackAction() const;
    bool isTriggerHackJump() const;
    bool isTriggerHackSwing() const;
    bool isTriggerHackEnd() const;
    bool isTriggerHackSeparateJump() const;
    bool isTriggerSeparateCapJangoHelp() const;
    bool isHoldHackSeparateJump() const;
    bool isTriggerGetOff() const;
    bool isHoldAction() const;
    bool isHoldJump() const;
    bool isHoldHipDrop() const;
    bool isTriggerStartTalk() const;
    bool isTriggerStartWorldWarp() const;
    bool isTriggerCancelWorldWarp() const;
    bool isTriggerSpinCap() const;
    bool isTriggerToggleStayCap() const;
    bool isTriggerSpinAttackSeparate() const;
    s32 getSeparatePlay1P();
    bool isTriggerCapReturn() const;
    bool isTriggerCapAttackSeparate() const;
    bool isTriggerSwingActionCap() const;
    bool isTriggerCapSingleHandThrow() const;
    bool isTriggerCapDoubleHandThrow() const;
    bool isTriggerCapSeparateJump() const;
    bool isTriggerCapSeparateHipDrop() const;
    bool isTriggerSwingPoleClimbFast() const;
    bool isHoldPoleClimbDown() const;
    bool isTriggerAppendCapAttack(bool) const;

    bool isHoldSpinCap() const;
    bool isHoldCapAction() const;
    bool isHoldPoleClimbFast() const;
    bool isHoldWallCatchMoveFast() const;
    bool isHoldHackAction() const;
    bool isHoldHackJump() const;

    bool isTriggerChange2D() const;
    bool isTriggerChange3D() const;
    bool isReleaseJump() const;
    bool isReleaseHackAction() const;
    bool isReleaseHackJump() const;
    bool isEnableDashInput() const;

    bool isThrowTypeSpiral(const sead::Vector2f&) const;
    bool isThrowTypeRolling(const sead::Vector2f&) const;

private:
    const al::LiveActor* mLiveActor;
    const IUsePlayerCollision* mPlayerCollision;
    const IUseDimension* mDimension;
    void* gap[11];
    al::SpinInputAnalyzer* mSpinInputAnalyzer;
    al::JoyPadAccelPoseAnalyzer* mJoyPadAccelPoseAnalyzer1;
    al::JoyPadAccelPoseAnalyzer* mJoyPadAccelPoseAnalyzer2;
    s32 _88;
    void* gap2[1];
    bool mDisableInput;
    void* gap3[20];
};
static_assert(sizeof(PlayerInput) == 0x140);
