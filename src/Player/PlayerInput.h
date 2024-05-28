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
    static s32 getSeparatePlay1P();
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

public:
    const al::LiveActor* mLiveActor;
    const IUsePlayerCollision* mPlayerCollision;
    const IUseDimension* mDimension;
    bool isMove = 0;
    int convergeInUpdateWallAlong = 0;
    float readInWalls = 0.0f;
    sead::Vector2f anotherVector2ForWalls = {0.0f, 0.0f};
    float somethingHere = 0.0f;
    sead::Vector2f anotherVector2ForWalls2 = {0.0f, 0.0f};
    bool hasSomeSnapMoveDirArea = false;
    sead::Vector3f someSnapMoveAreaDirVector = {0.0f, 0.0f, 0.0f};
    sead::Vector2f usedInUpdateForAngles = {0.0f, 0.0f};
    float usedInUpdateForSomething = 0.0f;
    float* arrayOfSizeF0 = nullptr;
    u64 constant60maybe = 0;
    u32 somethingAboutArray = 0;
    al::SpinInputAnalyzer* mSpinInputAnalyzer;
    al::JoyPadAccelPoseAnalyzer* mJoyPadAccelPoseAnalyzer1;
    al::JoyPadAccelPoseAnalyzer* mJoyPadAccelPoseAnalyzer2;
    int someIntConverged15 = 0;
    int somethingAboutP0 = 0;
    int somethingAboutP1 = 0;
    int convergedInUpdateToDisableInput = 0;
    bool mIsDisableInput = false;
    bool usedInUpdate = false;
    bool isInInvalidatePressStickCameraArea = false;
    bool flag2 = false;
    bool somethingFromUpdate = false;
    sead::Vector2f someVectorUsedInUpdate = {0.0f, 0.0f};
    sead::Vector3f someVectorEndOfUpdate = {0.0f, 0.0f, 0.0f};
    sead::Vector3f someVectorEndOfUpdate2 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f someVectorEndOfUpdate3 = {0.0f, 0.0f, 0.0f};
    bool someFlags[2] = {false, false};
    sead::Vector2f someVector2 = {0.0f, 0.0f};
    sead::Vector2f some2Vector = {0.0f, 0.0f};
    u64 placeholder4[3] = {0, 0, 0};
    sead::Vector3f someVector = {0.0f, 0.0f, 0.0f};
    sead::Vector3f someVector4 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f someVector5 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f someVector3 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f someVectorCollidedGroundPos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f someVectorCollidedGroundNormal = {0.0f, 0.0f, 0.0f};
};
static_assert(sizeof(PlayerInput) == 0x140);
