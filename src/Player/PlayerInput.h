#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
class SpinInputAnalyzer;
class JoyPadAccelPoseAnalyzer;
}  // namespace al
class IUsePlayerCollision;
class IUseDimension;

class PlayerInput {
public:
    PlayerInput(const al::LiveActor*, const IUsePlayerCollision*, const IUseDimension*);

    void update();
    void updateWallAlong();
    void updateSnapMoveArea();
    void updateInput3D();
    const sead::Vector2f& getMoveInputRaw(bool) const;
    void calcMoveInputImpl(sead::Vector3f*, const sead::Vector3f&, bool, bool, bool) const;
    void updateInput2D();
    void resetAlongWall();
    bool isNoInput() const;
    bool isNoInputSeparateCap() const;
    bool isHoldCapSeparateJump() const;
    bool isHoldCapSeparateHipDrop() const;
    s32 getSeparatePlay2P();
    bool isMove() const;
    bool isMoveDeepDown() const;
    const sead::Vector2f& getStickMoveRaw() const;
    bool isMoveDeepDownNoSnap() const;
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
    // TODO: proper parameter name
    bool isTriggerRolling(bool a1) const;
    bool isTriggerRollingRestartSwing() const;
    // TODO: proper parameter name
    bool isTriggerRollingCancelHipDrop(bool a1) const;
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
    // TODO: proper parameter name
    bool isTriggerAppendCapAttack(bool a1) const;
    bool isHoldSquat() const;
    void calcInputDirectionSnap2D(sead::Vector3f*) const;
    bool isHoldSpinCap() const;
    bool isHoldCapAction() const;
    bool isHoldPoleClimbFast() const;
    bool isHoldWallCatchMoveFast() const;
    bool isHoldHackAction() const;
    bool isHoldHackJump() const;
    bool isHoldEnterUpperDokan2D() const;
    bool isHoldEnterSideDokan2D(const sead::Vector3f&) const;
    void calcMoveDirection(sead::Vector3f*, const sead::Vector3f&) const;
    bool isTriggerChange2D() const;
    bool isTriggerChange3D() const;
    bool isReleaseJump() const;
    bool isReleaseHackAction() const;
    bool isReleaseHackJump() const;
    bool isEnableDashInput() const;
    bool isSpinInput() const;
    bool isSpinClockwise() const;
    bool isSpinCounterClockwise() const;
    void calcMoveInput(sead::Vector3f*, const sead::Vector3f&) const;
    void calc2DSnapJumpMoveInput(sead::Vector3f*, const sead::Vector3f&) const;
    bool isPoleMoveInputReverseX() const;
    const sead::Matrix34f& getInputViewMtx() const;
    void calcPoleMoveInput(sead::Vector2f*) const;
    void calcCapThrowInput(sead::Vector3f*, const sead::Vector3f&) const;
    void calcCapSeparateMoveInput(sead::Vector3f*, const sead::Vector3f&) const;
    bool isSameStickMove(const sead::Vector2f&, f32) const;
    f32 getRadiconInputSteeringValue() const;
    const sead::Vector2f& getStickCameraRaw() const;
    const sead::Vector2f& getCameraInputRaw() const;
    const sead::Vector2f& getStickCameraSubRaw() const;
    bool isTriggerCameraReset() const;
    bool isTriggerCameraSubjective() const;
    bool isHoldBalloonSet() const;
    bool isHoldCameraZoom() const;
    bool isHoldCameraSnapShotZoomIn() const;
    bool isHoldCameraSnapShotZoomOut() const;
    bool isHoldCameraSnapShotRollLeft() const;
    bool isHoldCameraSnapShotRollRight() const;
    bool isEnableShowTutorialInput() const;
    bool isEnableRecoveryLifeInput();
    const sead::Vector2f& getCapThrowDir() const;
    const sead::Vector2f& getSwingThrowDir() const;
    bool isSwingDirLeft() const;
    bool isSwingDirRight() const;
    bool isThrowTypeLeftRight(const sead::Vector2f&) const;
    // TODO: proper parameter name
    bool isThrowTypeSpiral(const sead::Vector2f& a1) const;
    // TODO: proper parameter name
    bool isThrowTypeRolling(const sead::Vector2f& a1) const;
    bool isEnableConsiderCapThrowDoubleSwing() const;
    f32 getPoseRotZDegreeLeft() const;
    f32 getPoseRotZDegreeRight() const;
    bool isTriggerSwingLeftHand() const;
    bool isTriggerSwingRightHand() const;
    bool isTriggerSwingDoubleHand() const;
    bool isTriggerSwingDoubleHandReverseDir() const;
    bool isTriggerSwingDoubleHandReverseInsideDir() const;
    bool isTriggerSwingDoubleHandReverseOutsideDir() const;
    const sead::Vector2f& getSwingVelLeftHand() const;
    const sead::Vector2f& getSwingVelRightHand() const;
    const sead::Vector2f& getSwingLeftHandDir() const;
    const sead::Vector2f& getSwingRightHandDir() const;
    void calcHoldMoveInput2D(sead::Vector3f*, const al::LiveActor*, const IUseDimension*,
                             const sead::Vector2f&, const sead::Vector3f&,
                             const sead::Matrix34f*) const;
    void calcHoldMoveInput3D(sead::Vector3f*, const sead::Vector3f&, const sead::Matrix34f*) const;
    void snapWallAlongInput(sead::Vector3f*, const sead::Vector3f&) const;
    void snapAreaInput(sead::Vector3f*, const sead::Vector3f&) const;
    void resetHoldInfo3D();
    void resetHoldInfo2D();

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
    bool mIsDisableInput;
    void* gap3[20];
};

static_assert(sizeof(PlayerInput) == 0x140);
