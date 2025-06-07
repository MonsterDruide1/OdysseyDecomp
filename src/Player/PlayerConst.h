#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class ByamlIter;
}

class PlayerConst : public al::HioNode {
public:
    PlayerConst();
    PlayerConst(const al::ByamlIter& byaml);

    virtual f32 getGravity() const { return mGravity; }

    virtual f32 getFrictionAttack() const { return mFrictionAttack; }

    virtual f32 getPushPower() const { return mPushPower; }

    virtual f32 getWaitPoseDegreeMax() const { return mWaitPoseDegreeMax; }

    virtual f32 getHillPoseDegreeMax() const { return mHillPoseDegreeMax; }

    virtual f32 getTiltPoseDegreeMax() const { return mTiltPoseDegreeMax; }

    virtual f32 getSlerpQuatRate() const { return mSlerpQuatRate; }

    virtual f32 getSlerpQuatRateWait() const { return mSlerpQuatRateWait; }

    virtual f32 getSlerpQuatGrav() const { return mSlerpQuatGrav; }

    virtual s32 getPreInputFrameCapThrow() const { return mPreInputFrameCapThrow; }

    virtual s32 getEnableActionFrameCapCatch() const { return mEnableActionFrameCapCatch; }

    virtual f32 getJumpPowerCapCatch() const { return mJumpPowerCapCatch; }

    virtual f32 getJumpGravityCapCatch() const { return mJumpGravityCapCatch; }

    virtual s32 getRunTimeContinuousThrow() const { return mRunTimeContinuousThrow; }

    virtual f32 getRunSpeedMaxContinuousThrow() const { return mRunSpeedMaxContinuousThrow; }

    virtual s32 getRunAccelFrameContinuousThrow() const { return mRunAccelFrameContinuousThrow; }

    virtual f32 getSeparateCheckHeight() const { return mSeparateCheckHeight; }

    virtual f32 getSeparateOffsetLerpRate() const { return mSeparateOffsetLerpRate; }

    virtual f32 getSeparateEnableThrowHeight() const { return mSeparateEnableThrowHeight; }

    virtual f32 getTall() const { return mTall; }

    virtual f32 getCollisionRadius() const { return mCollisionRadius; }

    virtual f32 getCollisionRadiusSquat() const { return mCollisionRadiusSquat; }

    virtual f32 getCollisionRadiusStand() const { return mCollisionRadiusStand; }

    virtual f32 getCollisionSmallStepHeight() const { return mCollisionSmallStepHeight; }

    virtual f32 getCollisionResetLimit() const { return mCollisionResetLimit; }

    virtual f32 getReflectCeilingPower() const { return mReflectCeilingPower; }

    virtual f32 getReflectTossPower() const { return mReflectTossPower; }

    virtual f32 getReflectUpperPunchScaleH() const { return mReflectUpperPunchScaleH; }

    virtual f32 getCollisionHitDownAngleH() const { return mCollisionHitDownAngleH; }

    virtual f32 getCollisionHitDownEscapeAngleV() const { return mCollisionHitDownEscapeAngleV; }

    virtual f32 getShadowDropHeightScale() const { return mShadowDropHeightScale; }

    virtual f32 getShadowDropNormalAdd() const { return mShadowDropNormalAdd; }

    virtual f32 getShadowDropLengthMin() const { return mShadowDropLengthMin; }

    virtual f32 getShadowDropLengthMax() const { return mShadowDropLengthMax; }

    virtual f32 getShadowDropLengthExtend() const { return mShadowDropLengthExtend; }

    virtual f32 getGravityDamage() const { return mGravityDamage; }

    virtual f32 getHopPowerDamage() const { return mHopPowerDamage; }

    virtual f32 getPushPowerDamage() const { return mPushPowerDamage; }

    virtual s32 getDamageCancelFrame() const { return mDamageCancelFrame; }

    virtual s32 getDamageInvalidCount() const { return mDamageInvalidCount; }

    virtual s32 getDamageInvalidCountRecovery() const { return mDamageInvalidCountRecovery; }

    virtual s32 getDamageInvalidCountAbyss() const { return mDamageInvalidCountAbyss; }

    virtual f32 getNormalMinSpeed2D() const { return mNormalMinSpeed2D; }

    virtual f32 getNormalMaxSpeed2D() const { return mNormalMaxSpeed2D; }

    virtual f32 getDashMaxSpeed2D() const { return mDashMaxSpeed2D; }

    virtual s32 getNormalAccelFrame2D() const { return mNormalAccelFrame2D; }

    virtual s32 getDashAccelFrame2D() const { return mDashAccelFrame2D; }

    virtual s32 getNormalDashAnimFrame2D() const { return mNormalDashAnimFrame2D; }

    virtual s32 getNormalBrakeFrame2D() const { return mNormalBrakeFrame2D; }

    virtual s32 getStickOnBrakeFrame2D() const { return mStickOnBrakeFrame2D; }

    virtual s32 getBrakeTurnStartFrame2D() const { return mBrakeTurnStartFrame2D; }

    virtual f32 getTurnEndSpeedRate2D() const { return mTurnEndSpeedRate2D; }

    virtual f32 getJumpPowerMin2DArea() const { return mJumpPowerMin2DArea; }

    virtual f32 getJumpPowerMax2DArea() const { return mJumpPowerMax2DArea; }

    virtual f32 getJumpPowerMinBorder2D() const { return mJumpPowerMinBorder2D; }

    virtual f32 getJumpPowerMaxBorder2D() const { return mJumpPowerMaxBorder2D; }

    virtual f32 getGravityMove() const { return mGravityMove; }

    virtual f32 getNormalMaxSpeed() const { return mNormalMaxSpeed; }

    virtual f32 getNormalMinSpeed() const { return mNormalMinSpeed; }

    virtual s32 getNormalAccelFrame() const { return mNormalAccelFrame; }

    virtual f32 getRunAccelAverageScale() const { return mRunAccelAverageScale; }

    virtual s32 getNormalBrakeFrame() const { return mNormalBrakeFrame; }

    virtual f32 getDashJudgeSpeed() const { return mDashJudgeSpeed; }

    virtual s32 getStickOnBrakeFrame() const { return mStickOnBrakeFrame; }

    virtual s32 getNormalDashAnimFrame() const { return mNormalDashAnimFrame; }

    virtual f32 getRunAfterTurnSpeedMax() const { return mRunAfterTurnSpeedMax; }

    virtual f32 getRunAfterTurnScale() const { return mRunAfterTurnScale; }

    virtual s32 getRunAfterTurnFrame() const { return mRunAfterTurnFrame; }

    virtual s32 getBrakeTurnStartFrame() const { return mBrakeTurnStartFrame; }

    virtual f32 getBrakeOnSpeedRate() const { return mBrakeOnSpeedRate; }

    virtual s32 getBrakeOnCounterBorder() const { return mBrakeOnCounterBorder; }

    virtual s32 getWallPushFrame() const { return mWallPushFrame; }

    virtual s32 getRunDeepDownFrame() const { return mRunDeepDownFrame; }

    virtual s32 getRunDeepDownMargine() const { return mRunDeepDownMargine; }

    virtual s32 getQuickTurnJumpFrame() const { return mQuickTurnJumpFrame; }

    virtual s32 getRoundAccelFrame() const { return mRoundAccelFrame; }

    virtual s32 getRoundBrakeFrame() const { return mRoundBrakeFrame; }

    virtual f32 getRoundFastDegree() const { return mRoundFastDegree; }

    virtual s32 getRoundAccelFrameFast() const { return mRoundAccelFrameFast; }

    virtual f32 getRoundMinDegree() const { return mRoundMinDegree; }

    virtual s32 getRoundBrakeFrameForce() const { return mRoundBrakeFrameForce; }

    virtual f32 getRoundFastDegreeForce() const { return mRoundFastDegreeForce; }

    virtual f32 getRoundLimitDegreeForce() const { return mRoundLimitDegreeForce; }

    virtual f32 getRoundLimitDegreeForceFast() const { return mRoundLimitDegreeForceFast; }

    virtual s32 getRoundAccelFrameForceFast() const { return mRoundAccelFrameForceFast; }

    virtual f32 getRoundLimitDegreeMin() const { return mRoundLimitDegreeMin; }

    virtual f32 getRoundLimitDegree() const { return mRoundLimitDegree; }

    virtual s32 getIceAccelFrame() const { return mIceAccelFrame; }

    virtual s32 getIceBrakeFrame() const { return mIceBrakeFrame; }

    virtual s32 getIceBrakeFrameHigh() const { return mIceBrakeFrameHigh; }

    virtual s32 getIceBrakeFrameWall() const { return mIceBrakeFrameWall; }

    virtual s32 getIceRoundAccelFrame() const { return mIceRoundAccelFrame; }

    virtual s32 getIceRoundAccelFrameFast() const { return mIceRoundAccelFrameFast; }

    virtual s32 getIceRoundBrakeFrame() const { return mIceRoundBrakeFrame; }

    virtual f32 getIceRoundFastDegree() const { return mIceRoundFastDegree; }

    virtual f32 getIceRoundMinDegree() const { return mIceRoundMinDegree; }

    virtual f32 getIceRoundLimitDegree() const { return mIceRoundLimitDegree; }

    virtual f32 getHillAddSpeed() const { return mHillAddSpeed; }

    virtual f32 getHillSubSpeed() const { return mHillSubSpeed; }

    virtual s32 getHillAccelAddFrame() const { return mHillAccelAddFrame; }

    virtual s32 getHillAccelSubFrame() const { return mHillAccelSubFrame; }

    virtual f32 getHillAccelSubAngleMin() const { return mHillAccelSubAngleMin; }

    virtual f32 getHillAccelSubAngleMax() const { return mHillAccelSubAngleMax; }

    virtual f32 getStandAngleMin() const { return mStandAngleMin; }

    virtual f32 getStandAngleMax() const { return mStandAngleMax; }

    virtual f32 getHillAngleSpeedMin() const { return mHillAngleSpeedMin; }

    virtual f32 getHillAngleSpeedMax() const { return mHillAngleSpeedMax; }

    virtual s32 getSpinCapThrowFrame() const { return mSpinCapThrowFrame; }

    virtual s32 getSpinCapThrowFrameAir() const { return mSpinCapThrowFrameAir; }

    virtual s32 getSpinCapThrowFrameSwim() const { return mSpinCapThrowFrameSwim; }

    virtual s32 getSpinCapThrowFrameSwing() const { return mSpinCapThrowFrameSwing; }

    virtual s32 getSpinCapThrowFrameContinuous() const { return mSpinCapThrowFrameContinuous; }

    virtual s32 getSpinAttackFrame() const { return mSpinAttackFrame; }

    virtual s32 getSpinBrakeFrame() const { return mSpinBrakeFrame; }

    virtual f32 getSpinAirJumpPower() const { return mSpinAirJumpPower; }

    virtual f32 getSpinAirSpeedMax() const { return mSpinAirSpeedMax; }

    virtual f32 getGravitySpinAir() const { return mGravitySpinAir; }

    virtual f32 getSlerpQuatRateSpinAir() const { return mSlerpQuatRateSpinAir; }

    virtual f32 getSpinBrakeRate() const { return mSpinBrakeRate; }

    virtual f32 getSpinBrakeSideAccel() const { return mSpinBrakeSideAccel; }

    virtual f32 getSpinBrakeSideBrakeRate() const { return mSpinBrakeSideBrakeRate; }

    virtual f32 getSpinBrakeSideMaxSpeedRate() const { return mSpinBrakeSideMaxSpeedRate; }

    virtual f32 getSpinRoundLimitDegree() const { return mSpinRoundLimitDegree; }

    virtual f32 getDamageFireJumpPower1st() const { return mDamageFireJumpPower1st; }

    virtual f32 getDamageFireJumpPower2nd() const { return mDamageFireJumpPower2nd; }

    virtual f32 getDamageFireJumpMoveSpeed() const { return mDamageFireJumpMoveSpeed; }

    virtual f32 getDamageFireCeilHitSpeed() const { return mDamageFireCeilHitSpeed; }

    virtual f32 getDamageFireGravity() const { return mDamageFireGravity; }

    virtual s32 getDamageFireNoGravityFrame() const { return mDamageFireNoGravityFrame; }

    virtual s32 getDamageFireRunTime() const { return mDamageFireRunTime; }

    virtual f32 getDamageFireRunSpeed() const { return mDamageFireRunSpeed; }

    virtual f32 getDamageFireRunBrakeFrame() const { return mDamageFireRunBrakeFrame; }

    virtual s32 getSandSinkDeadTime() const { return mSandSinkDeadTime; }

    virtual s32 getSandSinkBrakeHeightH() const { return mSandSinkBrakeHeightH; }

    virtual s32 getSandSinkBrakeHeightV() const { return mSandSinkBrakeHeightV; }

    virtual f32 getSandSinkHeight() const { return mSandSinkHeight; }

    virtual f32 getSandSinkCapThrow() const { return mSandSinkCapThrow; }

    virtual f32 getSandSinkBrakeMinH() const { return mSandSinkBrakeMinH; }

    virtual f32 getSandSinkBrakeMaxH() const { return mSandSinkBrakeMaxH; }

    virtual f32 getSandSinkBrakeMinV() const { return mSandSinkBrakeMinV; }

    virtual f32 getSandSinkBrakeMaxV() const { return mSandSinkBrakeMaxV; }

    virtual f32 getSlopeSlideAngleStart() const { return mSlopeSlideAngleStart; }

    virtual f32 getSlopeSlideAngleEnd() const { return mSlopeSlideAngleEnd; }

    virtual f32 getSlopeSlideAccel() const { return mSlopeSlideAccel; }

    virtual f32 getSlopeSlideBrake() const { return mSlopeSlideBrake; }

    virtual f32 getSlopeSlideMaxSpeed() const { return mSlopeSlideMaxSpeed; }

    virtual f32 getSlopeSlideSpeedEnd() const { return mSlopeSlideSpeedEnd; }

    virtual f32 getSlopeSlideSideAccel() const { return mSlopeSlideSideAccel; }

    virtual f32 getSlopeSlideSideBrake() const { return mSlopeSlideSideBrake; }

    virtual f32 getSlopeSlideSideMaxSpeed() const { return mSlopeSlideSideMaxSpeed; }

    virtual f32 getSlopeTurnDegree() const { return mSlopeTurnDegree; }

    virtual s32 getSlideInvalidFrame() const { return mSlideInvalidFrame; }

    virtual s32 getSlopeForceFrame() const { return mSlopeForceFrame; }

    virtual f32 getSlopeSlideForceSideAccel() const { return mSlopeSlideForceSideAccel; }

    virtual f32 getSlopeSlideForceSideBrake() const { return mSlopeSlideForceSideBrake; }

    virtual f32 getSlopeSlideForceSideMaxSpeed() const { return mSlopeSlideForceSideMaxSpeed; }

    virtual f32 getSlopeSlideForceTurnDegree() const { return mSlopeSlideForceTurnDegree; }

    virtual f32 getSlopeRollingSpeedStart() const { return mSlopeRollingSpeedStart; }

    virtual f32 getSlopeRollingSpeedBoost() const { return mSlopeRollingSpeedBoost; }

    virtual f32 getSlopeRollingMaxSpeed() const { return mSlopeRollingMaxSpeed; }

    virtual s32 getSlopeRollingFrameMinBoost() const { return mSlopeRollingFrameMinBoost; }

    virtual s32 getSlopeRollingFrameMin() const { return mSlopeRollingFrameMin; }

    virtual f32 getSlopeRollingStartJumpPower() const { return mSlopeRollingStartJumpPower; }

    virtual f32 getSlopeRollingStartSlideSpeed() const { return mSlopeRollingStartSlideSpeed; }

    virtual f32 getSlopeRollingAccel() const { return mSlopeRollingAccel; }

    virtual f32 getSlopeRollingBrake() const { return mSlopeRollingBrake; }

    virtual f32 getSlopeRollingAgainst() const { return mSlopeRollingAgainst; }

    virtual f32 getSlopeRollingAnglePowerMax() const { return mSlopeRollingAnglePowerMax; }

    virtual f32 getSlopeRollingSpeedEnd() const { return mSlopeRollingSpeedEnd; }

    virtual f32 getSlopeRollingSideAccel() const { return mSlopeRollingSideAccel; }

    virtual f32 getSlopeRollingSideBrake() const { return mSlopeRollingSideBrake; }

    virtual f32 getSlopeRollingSideMaxSpeed() const { return mSlopeRollingSideMaxSpeed; }

    virtual s32 getSlopeRollingUnRollFrame() const { return mSlopeRollingUnRollFrame; }

    virtual f32 getSlopeRollingEndBrake() const { return mSlopeRollingEndBrake; }

    virtual f32 getSlopeRollingEndBrakeEndSpeed() const { return mSlopeRollingEndBrakeEndSpeed; }

    virtual f32 getSlopeRollingReStartAccel() const { return mSlopeRollingReStartAccel; }

    virtual f32 getSlopeRollingReStartMaxAdd() const { return mSlopeRollingReStartMaxAdd; }

    virtual s32 getSlopeRollingReStartInterval() const { return mSlopeRollingReStartInterval; }

    virtual s32 getSlopeRollingReStartSwing() const { return mSlopeRollingReStartSwing; }

    virtual s32 getSlopeRollingReStartCharge() const { return mSlopeRollingReStartCharge; }

    virtual s32 getSlopeRollingReStartForce() const { return mSlopeRollingReStartForce; }

    virtual f32 getSlopeRollingAccelOnSkate() const { return mSlopeRollingAccelOnSkate; }

    virtual f32 getSlopeRollingSideAccelOnSkate() const { return mSlopeRollingSideAccelOnSkate; }

    virtual f32 getSlopeRollingBrakeOnSkate() const { return mSlopeRollingBrakeOnSkate; }

    virtual s32 getExtendFrame() const { return mExtendFrame; }

    virtual f32 getJumpInertiaRate() const { return mJumpInertiaRate; }

    virtual f32 getJumpPowerMin() const { return mJumpPowerMin; }

    virtual f32 getJumpPowerMax() const { return mJumpPowerMax; }

    virtual f32 getJumpGravity() const { return mJumpGravity; }

    virtual f32 getJumpBaseSpeedMax() const { return mJumpBaseSpeedMax; }

    virtual f32 getJumpMoveSpeedMin() const { return mJumpMoveSpeedMin; }

    virtual f32 getJumpMoveSpeedMax() const { return mJumpMoveSpeedMax; }

    virtual f32 getJumpAccelFront() const { return mJumpAccelFront; }

    virtual f32 getJumpAccelBack() const { return mJumpAccelBack; }

    virtual f32 getJumpAccelTurn() const { return mJumpAccelTurn; }

    virtual f32 getJumpTurnAngleStart() const { return mJumpTurnAngleStart; }

    virtual f32 getJumpTurnAngleLimit() const { return mJumpTurnAngleLimit; }

    virtual f32 getJumpTurnAngleFast() const { return mJumpTurnAngleFast; }

    virtual f32 getJumpTurnAngleFastLimit() const { return mJumpTurnAngleFastLimit; }

    virtual s32 getJumpTurnAccelFrame() const { return mJumpTurnAccelFrame; }

    virtual s32 getJumpTurnAccelFrameFast() const { return mJumpTurnAccelFrameFast; }

    virtual s32 getJumpTurnBrakeFrame() const { return mJumpTurnBrakeFrame; }

    virtual f32 getTrampleGravity() const { return mTrampleGravity; }

    virtual f32 getTrampleJumpPower() const { return mTrampleJumpPower; }

    virtual f32 getTrampleHighGravity() const { return mTrampleHighGravity; }

    virtual f32 getTrampleHighJumpPower() const { return mTrampleHighJumpPower; }

    virtual f32 getTrampleGravity2D() const { return mTrampleGravity2D; }

    virtual f32 getTrampleJumpPower2D() const { return mTrampleJumpPower2D; }

    virtual f32 getTrampleHighGravity2D() const { return mTrampleHighGravity2D; }

    virtual f32 getTrampleHighJumpPower2D() const { return mTrampleHighJumpPower2D; }

    virtual f32 getTrampleHipDropGravity() const { return mTrampleHipDropGravity; }

    virtual f32 getTrampleHipDropJumpPower() const { return mTrampleHipDropJumpPower; }

    virtual f32 getTrampleRisingBrakeVelH() const { return mTrampleRisingBrakeVelH; }

    virtual f32 getTrampleJumpCodePower() const { return mTrampleJumpCodePower; }

    virtual f32 getTrampleJumpCodePowerSmall() const { return mTrampleJumpCodePowerSmall; }

    virtual f32 getCapLeapFrogJumpGravity() const { return mCapLeapFrogJumpGravity; }

    virtual f32 getCapLeapFrogJumpPower() const { return mCapLeapFrogJumpPower; }

    virtual f32 getCapLeapFrogJumpPowerAir() const { return mCapLeapFrogJumpPowerAir; }

    virtual f32 getObjLeapFrogJumpPower() const { return mObjLeapFrogJumpPower; }

    virtual f32 getObjLeapFrogJumpPowerHigh() const { return mObjLeapFrogJumpPowerHigh; }

    virtual f32 getCapHeadSpringJumpGravity() const { return mCapHeadSpringJumpGravity; }

    virtual f32 getCapHeadSpringJumpPower() const { return mCapHeadSpringJumpPower; }

    virtual f32 getCapHeadSpringJumpGravityHigh() const { return mCapHeadSpringJumpGravityHigh; }

    virtual f32 getCapHeadSpringJumpPowerHigh() const { return mCapHeadSpringJumpPowerHigh; }

    virtual f32 getCapHeadSpringSpeedMax() const { return mCapHeadSpringSpeedMax; }

    virtual s32 getContinuousJumpPreInputFrame() const { return mContinuousJumpPreInputFrame; }

    virtual s32 getContinuousJumpCount() const { return mContinuousJumpCount; }

    virtual s32 getContinuousJumpTimer() const { return mContinuousJumpTimer; }

    virtual f32 getContinuousJumpPowerMin() const { return mContinuousJumpPowerMin; }

    virtual f32 getJumpPowerMax2nd() const { return mJumpPowerMax2nd; }

    virtual f32 getJumpGravity2nd() const { return mJumpGravity2nd; }

    virtual f32 getJumpPowerMax3rd() const { return mJumpPowerMax3rd; }

    virtual f32 getJumpGravity3rd() const { return mJumpGravity3rd; }

    virtual f32 getSpinFlowerJumpGravity() const { return mSpinFlowerJumpGravity; }

    virtual f32 getSpinFlowerJumpFallSpeedMax() const { return mSpinFlowerJumpFallSpeedMax; }

    virtual f32 getSpinFlowerJumpMovePower() const { return mSpinFlowerJumpMovePower; }

    virtual f32 getSpinFlowerJumpVelMax() const { return mSpinFlowerJumpVelMax; }

    virtual s32 getSpinFlowerJumpStayFrame() const { return mSpinFlowerJumpStayFrame; }

    virtual f32 getSpinFlowerJumpStaySpeedMax() const { return mSpinFlowerJumpStaySpeedMax; }

    virtual f32 getSpinFlowerJumpNoInputBrake() const { return mSpinFlowerJumpNoInputBrake; }

    virtual f32 getSpinFlowerJumpDownFallInitSpeed() const {
        return mSpinFlowerJumpDownFallInitSpeed;
    }

    virtual f32 getSpinFlowerJumpDownFallPower() const { return mSpinFlowerJumpDownFallPower; }

    virtual f32 getSpinFlowerJumpDownFallSpeedMax() const {
        return mSpinFlowerJumpDownFallSpeedMax;
    }

    virtual f32 getJumpGravityForceRun() const { return mJumpGravityForceRun; }

    virtual f32 getJumpPowerForceRun() const { return mJumpPowerForceRun; }

    virtual f32 getCapCatchPopPower() const { return mCapCatchPopPower; }

    virtual f32 getCapCatchPopGravity() const { return mCapCatchPopGravity; }

    virtual f32 getSquatJumpGravity() const { return mSquatJumpGravity; }

    virtual f32 getSquatJumpPower() const { return mSquatJumpPower; }

    virtual f32 getSquatJumpBackPower() const { return mSquatJumpBackPower; }

    virtual f32 getSquatJumpMovePowerFront() const { return mSquatJumpMovePowerFront; }

    virtual f32 getSquatJumpMovePowerSide() const { return mSquatJumpMovePowerSide; }

    virtual f32 getSquatJumpMoveSpeedMax() const { return mSquatJumpMoveSpeedMax; }

    virtual f32 getTurnJumpGravity() const { return mTurnJumpGravity; }

    virtual f32 getTurnJumpPower() const { return mTurnJumpPower; }

    virtual f32 getTurnJumpVelH() const { return mTurnJumpVelH; }

    virtual f32 getTurnJumpBrake() const { return mTurnJumpBrake; }

    virtual f32 getTurnJumpAccel() const { return mTurnJumpAccel; }

    virtual f32 getTurnJumpSideAccel() const { return mTurnJumpSideAccel; }

    virtual f32 getLongJumpAccel() const { return mLongJumpAccel; }

    virtual f32 getLongJumpBrake() const { return mLongJumpBrake; }

    virtual f32 getLongJumpSideAccel() const { return mLongJumpSideAccel; }

    virtual f32 getLongJumpGravity() const { return mLongJumpGravity; }

    virtual f32 getLongJumpJumpPow() const { return mLongJumpJumpPow; }

    virtual f32 getLongJumpMovePow() const { return mLongJumpMovePow; }

    virtual f32 getLongJumpInitSpeed() const { return mLongJumpInitSpeed; }

    virtual f32 getLongJumpSpeed() const { return mLongJumpSpeed; }

    virtual f32 getLongJumpSpeedMin() const { return mLongJumpSpeedMin; }

    virtual s32 getContinuousLongJumpCount() const { return mContinuousLongJumpCount; }

    virtual s32 getContinuousLongJumpTimer() const { return mContinuousLongJumpTimer; }

    virtual f32 getGravityAir() const { return mGravityAir; }

    virtual f32 getFrictionAir() const { return mFrictionAir; }

    virtual f32 getFallSpeedMax() const { return mFallSpeedMax; }

    virtual f32 getLongFallDistance() const { return mLongFallDistance; }

    virtual f32 getFallWallScaleVelocity() const { return mFallWallScaleVelocity; }

    virtual s32 getDownFallFrameMin() const { return mDownFallFrameMin; }

    virtual f32 getGravityWallSlide() const { return mGravityWallSlide; }

    virtual f32 getWallHeightLowLimit() const { return mWallHeightLowLimit; }

    virtual f32 getWallKeepDegree() const { return mWallKeepDegree; }

    virtual s32 getWallKeepFrame() const { return mWallKeepFrame; }

    virtual f32 getWallJumpGravity() const { return mWallJumpGravity; }

    virtual f32 getWallJumpHSpeed() const { return mWallJumpHSpeed; }

    virtual f32 getWallJumpPower() const { return mWallJumpPower; }

    virtual s32 getWallJumpInvalidateInputFrame() const { return mWallJumpInvalidateInputFrame; }

    virtual s32 getWallInhibitAfterPunch() const { return mWallInhibitAfterPunch; }

    virtual f32 getWallFollowAngleH() const { return mWallFollowAngleH; }

    virtual f32 getWallFollowAngleV() const { return mWallFollowAngleV; }

    virtual f32 getWallCatchDegree() const { return mWallCatchDegree; }

    virtual f32 getWallCatchHeightEdgeTop() const { return mWallCatchHeightEdgeTop; }

    virtual f32 getWallCatchHeightBottom() const { return mWallCatchHeightBottom; }

    virtual f32 getWallCatchKeepDegree() const { return mWallCatchKeepDegree; }

    virtual f32 getWallCatchMoveDegree() const { return mWallCatchMoveDegree; }

    virtual f32 getWallCatchMoveSpeed() const { return mWallCatchMoveSpeed; }

    virtual f32 getWallCatchMoveHeightRange() const { return mWallCatchMoveHeightRange; }

    virtual s32 getWallCatchMoveInterpolate() const { return mWallCatchMoveInterpolate; }

    virtual s32 getWallCatchMoveFrame() const { return mWallCatchMoveFrame; }

    virtual s32 getWallCatchMoveFrameFast() const { return mWallCatchMoveFrameFast; }

    virtual s32 getWallCatchMoveFrameSwing() const { return mWallCatchMoveFrameSwing; }

    virtual f32 getWallCatchInputRepeatAngle() const { return mWallCatchInputRepeatAngle; }

    virtual f32 getWallClimbDegree() const { return mWallClimbDegree; }

    virtual s32 getWallClimbJumpStartFrame() const { return mWallClimbJumpStartFrame; }

    virtual s32 getWallClimbJumpEndFrame() const { return mWallClimbJumpEndFrame; }

    virtual s32 getWallClimbStartFrame() const { return mWallClimbStartFrame; }

    virtual f32 getWallClimbGravity() const { return mWallClimbGravity; }

    virtual f32 getWallFallJumpSpeed() const { return mWallFallJumpSpeed; }

    virtual f32 getWallClimbJumpSpeedV() const { return mWallClimbJumpSpeedV; }

    virtual f32 getWallClimbJumpSpeedH() const { return mWallClimbJumpSpeedH; }

    virtual f32 getWallClimbJumpGravity() const { return mWallClimbJumpGravity; }

    virtual s32 getWallClimbJumpInvalidFrame() const { return mWallClimbJumpInvalidFrame; }

    virtual f32 getWallCatchHipLocalOffset() const { return mWallCatchHipLocalOffset; }

    virtual f32 getWallCatchHipStability() const { return mWallCatchHipStability; }

    virtual f32 getWallCatchHipFriction() const { return mWallCatchHipFriction; }

    virtual f32 getWallCatchHipLimitDegree() const { return mWallCatchHipLimitDegree; }

    virtual f32 getWallCatchStainAreaOffset() const { return mWallCatchStainAreaOffset; }

    virtual f32 getGrabCeilRange() const { return mGrabCeilRange; }

    virtual f32 getGrabCeilBodyRadius() const { return mGrabCeilBodyRadius; }

    virtual f32 getGrabCeilLeaveSpeedMin() const { return mGrabCeilLeaveSpeedMin; }

    virtual f32 getGrabCeilLeavePopPower() const { return mGrabCeilLeavePopPower; }

    virtual f32 getGrabCeilLeavePopGravity() const { return mGrabCeilLeavePopGravity; }

    virtual f32 getGrabCeilSwingStartOffset() const { return mGrabCeilSwingStartOffset; }

    virtual f32 getGrabCeilReverseInputBorder() const { return mGrabCeilReverseInputBorder; }

    virtual f32 getGrabCeilInputPowerBorder() const { return mGrabCeilInputPowerBorder; }

    virtual f32 getGrabCeilSwingWaitEnergy() const { return mGrabCeilSwingWaitEnergy; }

    virtual f32 getGrabCeilEnableJumpEnergy() const { return mGrabCeilEnableJumpEnergy; }

    virtual f32 getGrabCeilEnableJumpEnergyMax() const { return mGrabCeilEnableJumpEnergyMax; }

    virtual f32 getGrabCeilJumpForceAngle() const { return mGrabCeilJumpForceAngle; }

    virtual f32 getGrabCeilJumpPower() const { return mGrabCeilJumpPower; }

    virtual f32 getGrabCeilJumpMoveMin() const { return mGrabCeilJumpMoveMin; }

    virtual f32 getGrabCeilJumpMoveMax() const { return mGrabCeilJumpMoveMax; }

    virtual f32 getGrabCeilJumpGravity() const { return mGrabCeilJumpGravity; }

    virtual s32 getGrabCeilJumpInvalidFrame() const { return mGrabCeilJumpInvalidFrame; }

    virtual s32 getGrabCeilEnableNextFrame() const { return mGrabCeilEnableNextFrame; }

    virtual s32 getGrabCeilEnableFallSnapFrame() const { return mGrabCeilEnableFallSnapFrame; }

    virtual s32 getPoleClimbPreInputSwing() const { return mPoleClimbPreInputSwing; }

    virtual f32 getPoleClimbInputRepeatAngle() const { return mPoleClimbInputRepeatAngle; }

    virtual f32 getPoleClimbInputDegreeMove() const { return mPoleClimbInputDegreeMove; }

    virtual f32 getPoleClimbCatchRange() const { return mPoleClimbCatchRange; }

    virtual f32 getPoleClimbCatchRangeMin() const { return mPoleClimbCatchRangeMin; }

    virtual f32 getPoleClimbCatchRangeMax() const { return mPoleClimbCatchRangeMax; }

    virtual f32 getPoleClimbJointAngleMin() const { return mPoleClimbJointAngleMin; }

    virtual f32 getPoleClimbJointAngleMax() const { return mPoleClimbJointAngleMax; }

    virtual f32 getPoleClimbJointRangeMin() const { return mPoleClimbJointRangeMin; }

    virtual f32 getPoleClimbJointRangeMax() const { return mPoleClimbJointRangeMax; }

    virtual f32 getPoleClimbMoveWallDegree() const { return mPoleClimbMoveWallDegree; }

    virtual f32 getPoleClimbUpMargine() const { return mPoleClimbUpMargine; }

    virtual f32 getPoleClimbUpSpeed() const { return mPoleClimbUpSpeed; }

    virtual s32 getPoleClimbUpFrame() const { return mPoleClimbUpFrame; }

    virtual s32 getPoleClimbUpFrameFast() const { return mPoleClimbUpFrameFast; }

    virtual s32 getPoleClimbUpFrameSwing() const { return mPoleClimbUpFrameSwing; }

    virtual f32 getPoleClimbDownSpeed() const { return mPoleClimbDownSpeed; }

    virtual f32 getPoleClimbDownSpeedFast() const { return mPoleClimbDownSpeedFast; }

    virtual f32 getPoleClimbDownSpeedSwing() const { return mPoleClimbDownSpeedSwing; }

    virtual s32 getPoleClimbDownFrame() const { return mPoleClimbDownFrame; }

    virtual s32 getPoleClimbDownKeepTime() const { return mPoleClimbDownKeepTime; }

    virtual f32 getPoleClimbTurnDist() const { return mPoleClimbTurnDist; }

    virtual s32 getPoleClimbTurnFrame() const { return mPoleClimbTurnFrame; }

    virtual s32 getPoleClimbTurnStopFrame() const { return mPoleClimbTurnStopFrame; }

    virtual s32 getPoleTopStartFrame() const { return mPoleTopStartFrame; }

    virtual s32 getPoleTopEndFrame() const { return mPoleTopEndFrame; }

    virtual f32 getPoleTopTurnSpeed() const { return mPoleTopTurnSpeed; }

    virtual f32 getPoleTopEndUnderOffsetY() const { return mPoleTopEndUnderOffsetY; }

    virtual s32 getGroundSpinFrame() const { return mGroundSpinFrame; }

    virtual f32 getGroundSpinMoveSpeedMax() const { return mGroundSpinMoveSpeedMax; }

    virtual f32 getGroundSpinAccelRate() const { return mGroundSpinAccelRate; }

    virtual f32 getGroundSpinBrakeRate() const { return mGroundSpinBrakeRate; }

    virtual f32 getSpinJumpGravity() const { return mSpinJumpGravity; }

    virtual f32 getSpinJumpPower() const { return mSpinJumpPower; }

    virtual f32 getSpinJumpMoveSpeedMax() const { return mSpinJumpMoveSpeedMax; }

    virtual f32 getSpinJumpDownFallInitSpeed() const { return mSpinJumpDownFallInitSpeed; }

    virtual f32 getSpinJumpDownFallPower() const { return mSpinJumpDownFallPower; }

    virtual f32 getSpinJumpDownFallSpeedMax() const { return mSpinJumpDownFallSpeedMax; }

    virtual f32 getSquatBrakeEndSpeed() const { return mSquatBrakeEndSpeed; }

    virtual f32 getSquatAccelRate() const { return mSquatAccelRate; }

    virtual f32 getSquatBrakeRate() const { return mSquatBrakeRate; }

    virtual f32 getSquatBrakeRateOnSkate() const { return mSquatBrakeRateOnSkate; }

    virtual f32 getSquatBrakeSideAccel() const { return mSquatBrakeSideAccel; }

    virtual f32 getSquatBrakeSideRate() const { return mSquatBrakeSideRate; }

    virtual f32 getSquatBrakeSideAccelOnSkate() const { return mSquatBrakeSideAccelOnSkate; }

    virtual f32 getSquatBrakeSideRateOnSkate() const { return mSquatBrakeSideRateOnSkate; }

    virtual f32 getSquatBrakeSideMaxSpeedRate() const { return mSquatBrakeSideMaxSpeedRate; }

    virtual f32 getSquatWalkSpeed() const { return mSquatWalkSpeed; }

    virtual f32 getSquatWalkTurnSpeed() const { return mSquatWalkTurnSpeed; }

    virtual s32 getSquatWalkTurnFrame() const { return mSquatWalkTurnFrame; }

    virtual f32 getSquatJumpCeilSlideSpeed2D() const { return mSquatJumpCeilSlideSpeed2D; }

    virtual f32 getHipDropSpeed() const { return mHipDropSpeed; }

    virtual f32 getHipDropGravity() const { return mHipDropGravity; }

    virtual f32 getHipDropSpeedMax() const { return mHipDropSpeedMax; }

    virtual s32 getHipDropLandCancelFrame() const { return mHipDropLandCancelFrame; }

    virtual f32 getHipDropHeight() const { return mHipDropHeight; }

    virtual s32 getHipDropMsgInterval() const { return mHipDropMsgInterval; }

    virtual f32 getJumpHipDropPower() const { return mJumpHipDropPower; }

    virtual s32 getJumpHipDropPermitBeginFrame() const { return mJumpHipDropPermitBeginFrame; }

    virtual s32 getJumpHipDropPermitEndFrame() const { return mJumpHipDropPermitEndFrame; }

    virtual f32 getHeadSlidingSpeed() const { return mHeadSlidingSpeed; }

    virtual f32 getHeadSlidingSpeedMin() const { return mHeadSlidingSpeedMin; }

    virtual f32 getHeadSlidingBrake() const { return mHeadSlidingBrake; }

    virtual f32 getHeadSlidingSideAccel() const { return mHeadSlidingSideAccel; }

    virtual f32 getHeadSlidingJump() const { return mHeadSlidingJump; }

    virtual f32 getHeadSlidingGravityAir() const { return mHeadSlidingGravityAir; }

    virtual f32 getSwimCenterOffset() const { return mSwimCenterOffset; }

    virtual f32 getSwimWallCatchOffset() const { return mSwimWallCatchOffset; }

    virtual f32 getSwimRisePower() const { return mSwimRisePower; }

    virtual f32 getSwimRiseSpeedMax() const { return mSwimRiseSpeedMax; }

    virtual s32 getSwimRiseFrame() const { return mSwimRiseFrame; }

    virtual f32 getSwimGravity() const { return mSwimGravity; }

    virtual f32 getSwimGravityWalk() const { return mSwimGravityWalk; }

    virtual f32 getSwimFallSpeedMax() const { return mSwimFallSpeedMax; }

    virtual f32 getSwimFloorAccelH() const { return mSwimFloorAccelH; }

    virtual f32 getSwimFloorSpeedMaxH() const { return mSwimFloorSpeedMaxH; }

    virtual f32 getSwimHighAccelH() const { return mSwimHighAccelH; }

    virtual f32 getSwimHighSpeedMaxH() const { return mSwimHighSpeedMaxH; }

    virtual f32 getSwimLowAccelH() const { return mSwimLowAccelH; }

    virtual f32 getSwimLowSpeedMaxH() const { return mSwimLowSpeedMaxH; }

    virtual f32 getSwimBrakeRateH() const { return mSwimBrakeRateH; }

    virtual f32 getSwimWallHitSpeedMinH() const { return mSwimWallHitSpeedMinH; }

    virtual s32 getSwimHighAccelPermitFrame() const { return mSwimHighAccelPermitFrame; }

    virtual f32 getSwimFlowFieldBlend() const { return mSwimFlowFieldBlend; }

    virtual f32 getSwimWalkAnimMinRate() const { return mSwimWalkAnimMinRate; }

    virtual f32 getSwimWalkAnimMaxRate() const { return mSwimWalkAnimMaxRate; }

    virtual f32 getSwimWalkMaxSpeed() const { return mSwimWalkMaxSpeed; }

    virtual f32 getSwimSpinCapUpPower() const { return mSwimSpinCapUpPower; }

    virtual f32 getSwimSpinCapUpSpeedMax() const { return mSwimSpinCapUpSpeedMax; }

    virtual f32 getSwimRotStartAngle() const { return mSwimRotStartAngle; }

    virtual f32 getSwimRotFastAngle() const { return mSwimRotFastAngle; }

    virtual s32 getSwimRotAccelFrame() const { return mSwimRotAccelFrame; }

    virtual s32 getSwimRotAccelFrameFast() const { return mSwimRotAccelFrameFast; }

    virtual s32 getSwimRotBrakeFrame() const { return mSwimRotBrakeFrame; }

    virtual f32 getSwimRotSpeedChangeStart() const { return mSwimRotSpeedChangeStart; }

    virtual f32 getSwimRotSpeedForward() const { return mSwimRotSpeedForward; }

    virtual f32 getSwimRotSpeedMax() const { return mSwimRotSpeedMax; }

    virtual f32 getSwimSurfaceAccelH() const { return mSwimSurfaceAccelH; }

    virtual f32 getSwimSurfaceSpeedMaxH() const { return mSwimSurfaceSpeedMaxH; }

    virtual s32 getSwimSurfaceSpinCapFrame() const { return mSwimSurfaceSpinCapFrame; }

    virtual f32 getSwimSurfaceSpinCapSpeedMaxH() const { return mSwimSurfaceSpinCapSpeedMaxH; }

    virtual f32 getSwimSurfaceStartDist() const { return mSwimSurfaceStartDist; }

    virtual f32 getSwimSurfaceEndDist() const { return mSwimSurfaceEndDist; }

    virtual f32 getSwimSurfaceGravity() const { return mSwimSurfaceGravity; }

    virtual f32 getSwimSurfaceBaseHeight() const { return mSwimSurfaceBaseHeight; }

    virtual f32 getSwimSurfaceSpring() const { return mSwimSurfaceSpring; }

    virtual f32 getSwimSurfaceDamper() const { return mSwimSurfaceDamper; }

    virtual s32 getSwimSurfaceDamperStart() const { return mSwimSurfaceDamperStart; }

    virtual s32 getSwimSurfaceDamperFrame() const { return mSwimSurfaceDamperFrame; }

    virtual f32 getSwimSurfaceEnableJumpHeight() const { return mSwimSurfaceEnableJumpHeight; }

    virtual s32 getSwimSurfacePreInputJumpFrame() const { return mSwimSurfacePreInputJumpFrame; }

    virtual f32 getSwimSurfaceMoveSpring() const { return mSwimSurfaceMoveSpring; }

    virtual f32 getSwimSurfaceMoveDamper() const { return mSwimSurfaceMoveDamper; }

    virtual f32 getSwimSurfaceMoveBaseHeight() const { return mSwimSurfaceMoveBaseHeight; }

    virtual f32 getSwimRunSurfaceBaseHeight() const { return mSwimRunSurfaceBaseHeight; }

    virtual f32 getSwimRunSurfaceApproachRate() const { return mSwimRunSurfaceApproachRate; }

    virtual f32 getSwimRunSurfaceApproachLimit() const { return mSwimRunSurfaceApproachLimit; }

    virtual f32 getSwimRunSurfaceBrakeBorder() const { return mSwimRunSurfaceBrakeBorder; }

    virtual f32 getSwimRunSurfaceBrakeH() const { return mSwimRunSurfaceBrakeH; }

    virtual f32 getSwimRunSurfaceApproachBorderMax() const {
        return mSwimRunSurfaceApproachBorderMax;
    }

    virtual f32 getSwimRunSurfaceApproachBorderMin() const {
        return mSwimRunSurfaceApproachBorderMin;
    }

    virtual f32 getSwimRunSurfaceApproachRateMin() const { return mSwimRunSurfaceApproachRateMin; }

    virtual f32 getSwimFallInSpeed() const { return mSwimFallInSpeed; }

    virtual f32 getSwimFallInBrakeH() const { return mSwimFallInBrakeH; }

    virtual f32 getSwimFallInBrakeV() const { return mSwimFallInBrakeV; }

    virtual f32 getSwimHeadInBrakeH() const { return mSwimHeadInBrakeH; }

    virtual f32 getSwimHeadInBrakeV() const { return mSwimHeadInBrakeV; }

    virtual f32 getSwimHeadInRisePower() const { return mSwimHeadInRisePower; }

    virtual f32 getSwimHeadInRiseSpeedMax() const { return mSwimHeadInRiseSpeedMax; }

    virtual f32 getSwimHeadInSurfaceHeight() const { return mSwimHeadInSurfaceHeight; }

    virtual s32 getSwimFallInForceSurfaceFrame() const { return mSwimFallInForceSurfaceFrame; }

    virtual s32 getSwimFallInvalidJumpFrame() const { return mSwimFallInvalidJumpFrame; }

    virtual f32 getSwimDiveStartSpeed() const { return mSwimDiveStartSpeed; }

    virtual f32 getSwimDiveBrake() const { return mSwimDiveBrake; }

    virtual f32 getSwimDiveEndSpeed() const { return mSwimDiveEndSpeed; }

    virtual s32 getSwimDiveLandCount() const { return mSwimDiveLandCount; }

    virtual s32 getSwimDiveLandCancelFrame() const { return mSwimDiveLandCancelFrame; }

    virtual s32 getSwimDiveNoBrakeFrame() const { return mSwimDiveNoBrakeFrame; }

    virtual s32 getSwimDiveButtonValidFrame() const { return mSwimDiveButtonValidFrame; }

    virtual s32 getSwimDiveEndFrame() const { return mSwimDiveEndFrame; }

    virtual f32 getSwimDiveInBrakeH() const { return mSwimDiveInBrakeH; }

    virtual f32 getSwimDiveInBrakeV() const { return mSwimDiveInBrakeV; }

    virtual f32 getSwimDiveInRisePower() const { return mSwimDiveInRisePower; }

    virtual f32 getSwimDiveInRiseSpeedMax() const { return mSwimDiveInRiseSpeedMax; }

    virtual f32 getSwimDiveInSurfaceHeight() const { return mSwimDiveInSurfaceHeight; }

    virtual s32 getSwimDiveInKeepFrame() const { return mSwimDiveInKeepFrame; }

    virtual s32 getSwimHeadSlidingFrame() const { return mSwimHeadSlidingFrame; }

    virtual s32 getSwimHeadSlidingBrakeFrame() const { return mSwimHeadSlidingBrakeFrame; }

    virtual f32 getSwimHeadSlidingSpeed() const { return mSwimHeadSlidingSpeed; }

    virtual f32 getSwimHeadSlidingSpeedEnd() const { return mSwimHeadSlidingSpeedEnd; }

    virtual f32 getSwimHeadSlidingBrake() const { return mSwimHeadSlidingBrake; }

    virtual f32 getSwimHeadSlidingSideAccel() const { return mSwimHeadSlidingSideAccel; }

    virtual f32 getSwimHeadSlidingJump() const { return mSwimHeadSlidingJump; }

    virtual f32 getSwimHeadSlidingGravity() const { return mSwimHeadSlidingGravity; }

    virtual s32 getSwimHeadSlidingEndBrakeFrame() const { return mSwimHeadSlidingEndBrakeFrame; }

    virtual s32 getSwimHeadSlidingEndSpeedMin() const { return mSwimHeadSlidingEndSpeedMin; }

    virtual f32 getSwimJumpHipDropSpeed() const { return mSwimJumpHipDropSpeed; }

    virtual f32 getSwimJumpHipDropBrakeV() const { return mSwimJumpHipDropBrakeV; }

    virtual f32 getSwimJumpHipDropBrakeVCeiling() const { return mSwimJumpHipDropBrakeVCeiling; }

    virtual f32 getSwimJumpHipDropGravity() const { return mSwimJumpHipDropGravity; }

    virtual f32 getSwimJumpHipDropCancelSpeed() const { return mSwimJumpHipDropCancelSpeed; }

    virtual f32 getSwimJumpHipDropAccelH() const { return mSwimJumpHipDropAccelH; }

    virtual f32 getSwimJumpHipDropMoveSpeedH() const { return mSwimJumpHipDropMoveSpeedH; }

    virtual f32 getSwimJumpHipDropPopSpeed() const { return mSwimJumpHipDropPopSpeed; }

    virtual f32 getSwimJumpHipDropPopJumpAdd() const { return mSwimJumpHipDropPopJumpAdd; }

    virtual f32 getSwimTramplePower() const { return mSwimTramplePower; }

    virtual f32 getDiveTramplePower() const { return mDiveTramplePower; }

    virtual s32 getDiveTrampleCancelFrame() const { return mDiveTrampleCancelFrame; }

    virtual f32 getDamageSwimPushPower() const { return mDamageSwimPushPower; }

    virtual f32 getDamageSwimGravity() const { return mDamageSwimGravity; }

    virtual s32 getDamageSwimCancelFrame() const { return mDamageSwimCancelFrame; }

    virtual f32 getDamageSwimSurfaceGravity() const { return mDamageSwimSurfaceGravity; }

    virtual f32 getDamageSwimSurfaceHopPower() const { return mDamageSwimSurfaceHopPower; }

    virtual f32 getDamageSwimSurfacePushPower() const { return mDamageSwimSurfacePushPower; }

    virtual f32 getDamageSwimSurfaceLandSpeed() const { return mDamageSwimSurfaceLandSpeed; }

    virtual f32 getDamageSwimSurfaceLandBrake() const { return mDamageSwimSurfaceLandBrake; }

    virtual f32 getDamageSwimSurfaceLandEndSpeed() const { return mDamageSwimSurfaceLandEndSpeed; }

    virtual s32 getDamageSwimSurfaceCancelFrame() const { return mDamageSwimSurfaceCancelFrame; }

    virtual f32 getDamageSwimBrakeRateGround() const { return mDamageSwimBrakeRateGround; }

    virtual s32 getOxygenReduceFrame() const { return mOxygenReduceFrame; }

    virtual s32 getOxygenNoReduceFrame() const { return mOxygenNoReduceFrame; }

    virtual s32 getOxygenRecoveryFrame() const { return mOxygenRecoveryFrame; }

    virtual s32 getOxygenDamageInterval() const { return mOxygenDamageInterval; }

    virtual s32 getIceWaterDamageInterval() const { return mIceWaterDamageInterval; }

    virtual s32 getIceWaterRecoveryFrame() const { return mIceWaterRecoveryFrame; }

    virtual f32 getMoveAnimSpeedMax() const { return mMoveAnimSpeedMax; }

    virtual f32 getAnimFrameRateSpeedMin() const { return mAnimFrameRateSpeedMin; }

    virtual f32 getRunBorderSpeed() const { return mRunBorderSpeed; }

    virtual f32 getRunBlendRange() const { return mRunBlendRange; }

    virtual f32 getDashBorderSpeed() const { return mDashBorderSpeed; }

    virtual f32 getDashBlendRange() const { return mDashBlendRange; }

    virtual f32 getDashFastBorderSpeed() const { return mDashFastBorderSpeed; }

    virtual f32 getDashFastBlendRange() const { return mDashFastBlendRange; }

    virtual f32 getAnimFrameRateSpeedMax() const { return mAnimFrameRateSpeedMax; }

    virtual f32 getAnimFrameRateRunStart() const { return mAnimFrameRateRunStart; }

    virtual f32 getAnimFrameRateMinRun() const { return mAnimFrameRateMinRun; }

    virtual f32 getAnimFrameRateMaxRun() const { return mAnimFrameRateMaxRun; }

    virtual f32 getAnimFrameRateMaxDash() const { return mAnimFrameRateMaxDash; }

    virtual f32 getAnimFrameRateMaxDashFast() const { return mAnimFrameRateMaxDashFast; }

    virtual f32 getRunStartPlayFrameScale() const { return mRunStartPlayFrameScale; }

    virtual s32 getRunStartBlendFrame() const { return mRunStartBlendFrame; }

    virtual f32 getDamageFireRunAnimRate() const { return mDamageFireRunAnimRate; }

    virtual f32 getRunSkateAnimSpeedOffset() const { return mRunSkateAnimSpeedOffset; }

    virtual f32 getAnimFrameRateRange2D() const { return mAnimFrameRateRange2D; }

    virtual f32 getAnimFrameRateMinRun2D() const { return mAnimFrameRateMinRun2D; }

    virtual f32 getAnimFrameRateMaxRun2D() const { return mAnimFrameRateMaxRun2D; }

    virtual s32 getIKBlendFrameRun() const { return mIKBlendFrameRun; }

    virtual f32 getIKBlendRateRunMin() const { return mIKBlendRateRunMin; }

    virtual f32 getIKBlendRateRunMax() const { return mIKBlendRateRunMax; }

    virtual f32 getRollingAnimBorderSpeedMin() const { return mRollingAnimBorderSpeedMin; }

    virtual f32 getRollingAnimBorderSpeedMax() const { return mRollingAnimBorderSpeedMax; }

    virtual f32 getRollingAnimFrameRateMin() const { return mRollingAnimFrameRateMin; }

    virtual f32 getRollingAnimFrameRateMax() const { return mRollingAnimFrameRateMax; }

    virtual s32 getSwimPaddleAnimInterval() const { return mSwimPaddleAnimInterval; }

    virtual s32 getSwimPaddleAnimRateIntervalMax() const { return mSwimPaddleAnimRateIntervalMax; }

    virtual s32 getSwimPaddleAnimRateIntervalMin() const { return mSwimPaddleAnimRateIntervalMin; }

    virtual f32 getSwimPaddleAnimMaxRate() const { return mSwimPaddleAnimMaxRate; }

    virtual f32 getSwimBentForwardMax() const { return mSwimBentForwardMax; }

    virtual f32 getSwimBentForwardBlendRate() const { return mSwimBentForwardBlendRate; }

    virtual f32 getSwimBentSideMax() const { return mSwimBentSideMax; }

    virtual f32 getSwimBentSpineMax() const { return mSwimBentSpineMax; }

    virtual f32 getSwimBentSideBlendRate() const { return mSwimBentSideBlendRate; }

    virtual f32 getSwimBentFrontMax() const { return mSwimBentFrontMax; }

    virtual f32 getSwimBentFrontBlendRate() const { return mSwimBentFrontBlendRate; }

    virtual f32 getSwimWalkAnimSpeedMax() const { return mSwimWalkAnimSpeedMax; }

    virtual f32 getSwimWalkAnimSpeedMin() const { return mSwimWalkAnimSpeedMin; }

    virtual f32 getSwimWalkAnimFrameRateMax() const { return mSwimWalkAnimFrameRateMax; }

    virtual f32 getSwimWalkAnimFrameRateMin() const { return mSwimWalkAnimFrameRateMin; }

    virtual f32 getSandSinkBorderMin() const { return mSandSinkBorderMin; }

    virtual f32 getSandSinkBorderMax() const { return mSandSinkBorderMax; }

    virtual f32 getSandSinkBorderRateMin() const { return mSandSinkBorderRateMin; }

    virtual f32 getSandSinkBorderRateMax() const { return mSandSinkBorderRateMax; }

    virtual f32 getSandSinkFrameRateMin() const { return mSandSinkFrameRateMin; }

    virtual f32 getSandSinkFrameRateMax() const { return mSandSinkFrameRateMax; }

    virtual s32 getLookAtEyeKeepFrame() const { return mLookAtEyeKeepFrame; }

    virtual s32 getLookAtEyeKeepFrameInSight() const { return mLookAtEyeKeepFrameInSight; }

    virtual s32 getLookAtEyeKeepFrameWait() const { return mLookAtEyeKeepFrameWait; }

    virtual f32 getLookAtEyeDistance() const { return mLookAtEyeDistance; }

    virtual f32 getLookAtEyeAngleMinH() const { return mLookAtEyeAngleMinH; }

    virtual f32 getLookAtEyeAngleMinInSightH() const { return mLookAtEyeAngleMinInSightH; }

    virtual f32 getLookAtEyeAngleMaxH() const { return mLookAtEyeAngleMaxH; }

    virtual f32 getLookAtEyeAngleMinV() const { return mLookAtEyeAngleMinV; }

    virtual f32 getLookAtEyeAngleMinInSightV() const { return mLookAtEyeAngleMinInSightV; }

    virtual f32 getLookAtEyeAngleMaxV() const { return mLookAtEyeAngleMaxV; }

    virtual f32 getTiltEyeBorderStart() const { return mTiltEyeBorderStart; }

    virtual f32 getTiltEyeBorderEnd() const { return mTiltEyeBorderEnd; }

    virtual f32 getTiltEyeAngleScale() const { return mTiltEyeAngleScale; }

    virtual f32 getCenterTiltRateMax() const { return mCenterTiltRateMax; }

    virtual f32 getNoseChildLocalOffset() const { return mNoseChildLocalOffset; }

    virtual f32 getNoseStability() const { return mNoseStability; }

    virtual f32 getNoseFriction() const { return mNoseFriction; }

    virtual f32 getNoseLimitDegree() const { return mNoseLimitDegree; }

    virtual f32 getMustacheChildLocalOffset() const { return mMustacheChildLocalOffset; }

    virtual f32 getMustacheStability() const { return mMustacheStability; }

    virtual f32 getMustacheFriction() const { return mMustacheFriction; }

    virtual f32 getMustacheLimitDegree() const { return mMustacheLimitDegree; }

    virtual s32 getCapInterpolateFrame() const { return mCapInterpolateFrame; }

    virtual f32 getCapChildLocalOffset() const { return mCapChildLocalOffset; }

    virtual f32 getCapStability() const { return mCapStability; }

    virtual f32 getCapFriction() const { return mCapFriction; }

    virtual f32 getCapLimitDegree() const { return mCapLimitDegree; }

    virtual f32 getCapTransStability() const { return mCapTransStability; }

    virtual f32 getCapTransFriction() const { return mCapTransFriction; }

    virtual f32 getCapTransLimit() const { return mCapTransLimit; }

    virtual s32 getCapManHeroEyesWaitAppearFrame() const { return mCapManHeroEyesWaitAppearFrame; }

    virtual s32 getDeadWipeStartDamage() const { return mDeadWipeStartDamage; }

    virtual s32 getDeadWipeWaitDamage() const { return mDeadWipeWaitDamage; }

    virtual s32 getDeadWipeStartAbyss() const { return mDeadWipeStartAbyss; }

    virtual s32 getDeadWipeWaitAbyss() const { return mDeadWipeWaitAbyss; }

    virtual s32 getDeadWipeStartAbyssWithCapMsg() const { return mDeadWipeStartAbyssWithCapMsg; }

    virtual s32 getDeadWipeWaitAbyssWithCapMsg() const { return mDeadWipeWaitAbyssWithCapMsg; }

    virtual s32 getDeadWipeStartPress() const { return mDeadWipeStartPress; }

    virtual s32 getDeadWipeWaitPress() const { return mDeadWipeWaitPress; }

    virtual s32 getDeadWipeStartSandSink() const { return mDeadWipeStartSandSink; }

    virtual s32 getDeadWipeWaitSandSink() const { return mDeadWipeWaitSandSink; }

    virtual s32 getDeadWipeStartNoOxygen() const { return mDeadWipeStartNoOxygen; }

    virtual s32 getDeadWipeWaitNoOxygen() const { return mDeadWipeWaitNoOxygen; }

    virtual s32 getDeadWipeStartIceWater() const { return mDeadWipeStartIceWater; }

    virtual s32 getDeadWipeWaitIceWater() const { return mDeadWipeWaitIceWater; }

    virtual f32 getCoinDashSpeed() const { return mCoinDashSpeed; }

    virtual f32 getCoinDashSpeedLimit() const { return mCoinDashSpeedLimit; }

    virtual f32 getAdditionalSpeedLimit() const { return mAdditionalSpeedLimit; }

private:
    f32 mGravity;
    f32 mFrictionAttack;
    f32 mPushPower;
    f32 mWaitPoseDegreeMax;
    f32 mHillPoseDegreeMax;
    f32 mTiltPoseDegreeMax;
    f32 mSlerpQuatRate;
    f32 mSlerpQuatRateWait;
    f32 mSlerpQuatGrav;
    s32 mPreInputFrameCapThrow;
    s32 mEnableActionFrameCapCatch;
    f32 mJumpPowerCapCatch;
    f32 mJumpGravityCapCatch;
    s32 mRunTimeContinuousThrow;
    f32 mRunSpeedMaxContinuousThrow;
    s32 mRunAccelFrameContinuousThrow;
    f32 mSeparateCheckHeight;
    f32 mSeparateOffsetLerpRate;
    f32 mSeparateEnableThrowHeight;
    f32 mTall;
    f32 mCollisionRadius;
    f32 mCollisionRadiusSquat;
    f32 mCollisionRadiusStand;
    f32 mCollisionSmallStepHeight;
    f32 mCollisionResetLimit;
    f32 mReflectCeilingPower;
    f32 mReflectTossPower;
    f32 mReflectUpperPunchScaleH;
    f32 mCollisionHitDownAngleH;
    f32 mCollisionHitDownEscapeAngleV;
    f32 mShadowDropHeightScale;
    f32 mShadowDropNormalAdd;
    f32 mShadowDropLengthMin;
    f32 mShadowDropLengthMax;
    f32 mShadowDropLengthExtend;
    f32 mGravityDamage;
    f32 mHopPowerDamage;
    f32 mPushPowerDamage;
    s32 mDamageCancelFrame;
    s32 mDamageInvalidCount;
    s32 mDamageInvalidCountRecovery;
    s32 mDamageInvalidCountAbyss;
    f32 mNormalMinSpeed2D;
    f32 mNormalMaxSpeed2D;
    f32 mDashMaxSpeed2D;
    s32 mNormalAccelFrame2D;
    s32 mDashAccelFrame2D;
    s32 mNormalDashAnimFrame2D;
    s32 mNormalBrakeFrame2D;
    s32 mStickOnBrakeFrame2D;
    s32 mBrakeTurnStartFrame2D;
    f32 mTurnEndSpeedRate2D;
    f32 mJumpPowerMin2DArea;
    f32 mJumpPowerMax2DArea;
    f32 mJumpPowerMinBorder2D;
    f32 mJumpPowerMaxBorder2D;
    f32 mGravityMove;
    f32 mNormalMaxSpeed;
    f32 mNormalMinSpeed;
    s32 mNormalAccelFrame;
    f32 mRunAccelAverageScale;
    s32 mNormalBrakeFrame;
    f32 mDashJudgeSpeed;
    s32 mStickOnBrakeFrame;
    s32 mNormalDashAnimFrame;
    f32 mRunAfterTurnSpeedMax;
    f32 mRunAfterTurnScale;
    s32 mRunAfterTurnFrame;
    s32 mBrakeTurnStartFrame;
    f32 mBrakeOnSpeedRate;
    s32 mBrakeOnCounterBorder;
    s32 mWallPushFrame;
    s32 mRunDeepDownFrame;
    s32 mRunDeepDownMargine;
    s32 mQuickTurnJumpFrame;
    s32 mRoundAccelFrame;
    s32 mRoundBrakeFrame;
    f32 mRoundFastDegree;
    s32 mRoundAccelFrameFast;
    f32 mRoundMinDegree;
    s32 mRoundBrakeFrameForce;
    f32 mRoundFastDegreeForce;
    f32 mRoundLimitDegreeForce;
    f32 mRoundLimitDegreeForceFast;
    s32 mRoundAccelFrameForceFast;
    f32 mRoundLimitDegreeMin;
    f32 mRoundLimitDegree;
    s32 mIceAccelFrame;
    s32 mIceBrakeFrame;
    s32 mIceBrakeFrameHigh;
    s32 mIceBrakeFrameWall;
    s32 mIceRoundAccelFrame;
    s32 mIceRoundAccelFrameFast;
    s32 mIceRoundBrakeFrame;
    f32 mIceRoundFastDegree;
    f32 mIceRoundMinDegree;
    f32 mIceRoundLimitDegree;
    f32 mHillAddSpeed;
    f32 mHillSubSpeed;
    s32 mHillAccelAddFrame;
    s32 mHillAccelSubFrame;
    f32 mHillAccelSubAngleMin;
    f32 mHillAccelSubAngleMax;
    f32 mStandAngleMin;
    f32 mStandAngleMax;
    f32 mHillAngleSpeedMin;
    f32 mHillAngleSpeedMax;
    s32 mSpinCapThrowFrame;
    s32 mSpinCapThrowFrameAir;
    s32 mSpinCapThrowFrameSwim;
    s32 mSpinCapThrowFrameSwing;
    s32 mSpinCapThrowFrameContinuous;
    s32 mSpinAttackFrame;
    s32 mSpinBrakeFrame;
    f32 mSpinAirJumpPower;
    f32 mSpinAirSpeedMax;
    f32 mGravitySpinAir;
    f32 mSlerpQuatRateSpinAir;
    f32 mSpinBrakeRate;
    f32 mSpinBrakeSideAccel;
    f32 mSpinBrakeSideBrakeRate;
    f32 mSpinBrakeSideMaxSpeedRate;
    f32 mSpinRoundLimitDegree;
    f32 mDamageFireJumpPower1st;
    f32 mDamageFireJumpPower2nd;
    f32 mDamageFireJumpMoveSpeed;
    f32 mDamageFireCeilHitSpeed;
    f32 mDamageFireGravity;
    s32 mDamageFireNoGravityFrame;
    s32 mDamageFireRunTime;
    f32 mDamageFireRunSpeed;
    f32 mDamageFireRunBrakeFrame;
    s32 mSandSinkDeadTime;
    s32 mSandSinkBrakeHeightH;
    s32 mSandSinkBrakeHeightV;
    f32 mSandSinkHeight;
    f32 mSandSinkCapThrow;
    f32 mSandSinkBrakeMinH;
    f32 mSandSinkBrakeMaxH;
    f32 mSandSinkBrakeMinV;
    f32 mSandSinkBrakeMaxV;
    f32 mSlopeSlideAngleStart;
    f32 mSlopeSlideAngleEnd;
    f32 mSlopeSlideAccel;
    f32 mSlopeSlideBrake;
    f32 mSlopeSlideMaxSpeed;
    f32 mSlopeSlideSpeedEnd;
    f32 mSlopeSlideSideAccel;
    f32 mSlopeSlideSideBrake;
    f32 mSlopeSlideSideMaxSpeed;
    f32 mSlopeTurnDegree;
    s32 mSlideInvalidFrame;
    s32 mSlopeForceFrame;
    f32 mSlopeSlideForceSideAccel;
    f32 mSlopeSlideForceSideBrake;
    f32 mSlopeSlideForceSideMaxSpeed;
    f32 mSlopeSlideForceTurnDegree;
    f32 mSlopeRollingSpeedStart;
    f32 mSlopeRollingSpeedBoost;
    f32 mSlopeRollingMaxSpeed;
    s32 mSlopeRollingFrameMinBoost;
    s32 mSlopeRollingFrameMin;
    f32 mSlopeRollingStartJumpPower;
    f32 mSlopeRollingStartSlideSpeed;
    f32 mSlopeRollingAccel;
    f32 mSlopeRollingBrake;
    f32 mSlopeRollingAgainst;
    f32 mSlopeRollingAnglePowerMax;
    f32 mSlopeRollingSpeedEnd;
    f32 mSlopeRollingSideAccel;
    f32 mSlopeRollingSideBrake;
    f32 mSlopeRollingSideMaxSpeed;
    s32 mSlopeRollingUnRollFrame;
    f32 mSlopeRollingEndBrake;
    f32 mSlopeRollingEndBrakeEndSpeed;
    f32 mSlopeRollingReStartAccel;
    f32 mSlopeRollingReStartMaxAdd;
    s32 mSlopeRollingReStartInterval;
    s32 mSlopeRollingReStartSwing;
    s32 mSlopeRollingReStartCharge;
    s32 mSlopeRollingReStartForce;
    f32 mSlopeRollingAccelOnSkate;
    f32 mSlopeRollingSideAccelOnSkate;
    f32 mSlopeRollingBrakeOnSkate;
    s32 mExtendFrame;
    f32 mJumpInertiaRate;
    f32 mJumpPowerMin;
    f32 mJumpPowerMax;
    f32 mJumpGravity;
    f32 mJumpBaseSpeedMax;
    f32 mJumpMoveSpeedMin;
    f32 mJumpMoveSpeedMax;
    f32 mJumpAccelFront;
    f32 mJumpAccelBack;
    f32 mJumpAccelTurn;
    f32 mJumpTurnAngleStart;
    f32 mJumpTurnAngleLimit;
    f32 mJumpTurnAngleFast;
    f32 mJumpTurnAngleFastLimit;
    s32 mJumpTurnAccelFrame;
    s32 mJumpTurnAccelFrameFast;
    s32 mJumpTurnBrakeFrame;
    f32 mTrampleGravity;
    f32 mTrampleJumpPower;
    f32 mTrampleHighGravity;
    f32 mTrampleHighJumpPower;
    f32 mTrampleGravity2D;
    f32 mTrampleJumpPower2D;
    f32 mTrampleHighGravity2D;
    f32 mTrampleHighJumpPower2D;
    f32 mTrampleHipDropGravity;
    f32 mTrampleHipDropJumpPower;
    f32 mTrampleRisingBrakeVelH;
    f32 mTrampleJumpCodePower;
    f32 mTrampleJumpCodePowerSmall;
    f32 mCapLeapFrogJumpGravity;
    f32 mCapLeapFrogJumpPower;
    f32 mCapLeapFrogJumpPowerAir;
    f32 mObjLeapFrogJumpPower;
    f32 mObjLeapFrogJumpPowerHigh;
    f32 mCapHeadSpringJumpGravity;
    f32 mCapHeadSpringJumpPower;
    f32 mCapHeadSpringJumpGravityHigh;
    f32 mCapHeadSpringJumpPowerHigh;
    f32 mCapHeadSpringSpeedMax;
    s32 mContinuousJumpPreInputFrame;
    s32 mContinuousJumpCount;
    s32 mContinuousJumpTimer;
    f32 mContinuousJumpPowerMin;
    f32 mJumpPowerMax2nd;
    f32 mJumpGravity2nd;
    f32 mJumpPowerMax3rd;
    f32 mJumpGravity3rd;
    f32 mSpinFlowerJumpGravity;
    f32 mSpinFlowerJumpFallSpeedMax;
    f32 mSpinFlowerJumpMovePower;
    f32 mSpinFlowerJumpVelMax;
    s32 mSpinFlowerJumpStayFrame;
    f32 mSpinFlowerJumpStaySpeedMax;
    f32 mSpinFlowerJumpNoInputBrake;
    f32 mSpinFlowerJumpDownFallInitSpeed;
    f32 mSpinFlowerJumpDownFallPower;
    f32 mSpinFlowerJumpDownFallSpeedMax;
    f32 mJumpGravityForceRun;
    f32 mJumpPowerForceRun;
    f32 mCapCatchPopPower;
    f32 mCapCatchPopGravity;
    f32 mSquatJumpGravity;
    f32 mSquatJumpPower;
    f32 mSquatJumpBackPower;
    f32 mSquatJumpMovePowerFront;
    f32 mSquatJumpMovePowerSide;
    f32 mSquatJumpMoveSpeedMax;
    f32 mTurnJumpGravity;
    f32 mTurnJumpPower;
    f32 mTurnJumpVelH;
    f32 mTurnJumpBrake;
    f32 mTurnJumpAccel;
    f32 mTurnJumpSideAccel;
    f32 mLongJumpAccel;
    f32 mLongJumpBrake;
    f32 mLongJumpSideAccel;
    f32 mLongJumpGravity;
    f32 mLongJumpJumpPow;
    f32 mLongJumpMovePow;
    f32 mLongJumpInitSpeed;
    f32 mLongJumpSpeed;
    f32 mLongJumpSpeedMin;
    s32 mContinuousLongJumpCount;
    s32 mContinuousLongJumpTimer;
    f32 mGravityAir;
    f32 mFrictionAir;
    f32 mFallSpeedMax;
    f32 mLongFallDistance;
    f32 mFallWallScaleVelocity;
    s32 mDownFallFrameMin;
    f32 mGravityWallSlide;
    f32 mWallHeightLowLimit;
    f32 mWallKeepDegree;
    s32 mWallKeepFrame;
    f32 mWallJumpGravity;
    f32 mWallJumpHSpeed;
    f32 mWallJumpPower;
    s32 mWallJumpInvalidateInputFrame;
    s32 mWallInhibitAfterPunch;
    f32 mWallFollowAngleH;
    f32 mWallFollowAngleV;
    f32 mWallCatchDegree;
    f32 mWallCatchHeightEdgeTop;
    f32 mWallCatchHeightBottom;
    f32 mWallCatchKeepDegree;
    f32 mWallCatchMoveDegree;
    f32 mWallCatchMoveSpeed;
    f32 mWallCatchMoveHeightRange;
    s32 mWallCatchMoveInterpolate;
    s32 mWallCatchMoveFrame;
    s32 mWallCatchMoveFrameFast;
    s32 mWallCatchMoveFrameSwing;
    f32 mWallCatchInputRepeatAngle;
    f32 mWallClimbDegree;
    s32 mWallClimbJumpStartFrame;
    s32 mWallClimbJumpEndFrame;
    s32 mWallClimbStartFrame;
    f32 mWallClimbGravity;
    f32 mWallFallJumpSpeed;
    f32 mWallClimbJumpSpeedV;
    f32 mWallClimbJumpSpeedH;
    f32 mWallClimbJumpGravity;
    s32 mWallClimbJumpInvalidFrame;
    f32 mWallCatchHipLocalOffset;
    f32 mWallCatchHipStability;
    f32 mWallCatchHipFriction;
    f32 mWallCatchHipLimitDegree;
    f32 mWallCatchStainAreaOffset;
    f32 mGrabCeilRange;
    f32 mGrabCeilBodyRadius;
    f32 mGrabCeilLeaveSpeedMin;
    f32 mGrabCeilLeavePopPower;
    f32 mGrabCeilLeavePopGravity;
    f32 mGrabCeilSwingStartOffset;
    f32 mGrabCeilReverseInputBorder;
    f32 mGrabCeilInputPowerBorder;
    f32 mGrabCeilSwingWaitEnergy;
    f32 mGrabCeilEnableJumpEnergy;
    f32 mGrabCeilEnableJumpEnergyMax;
    f32 mGrabCeilJumpForceAngle;
    f32 mGrabCeilJumpPower;
    f32 mGrabCeilJumpMoveMin;
    f32 mGrabCeilJumpMoveMax;
    f32 mGrabCeilJumpGravity;
    s32 mGrabCeilJumpInvalidFrame;
    s32 mGrabCeilEnableNextFrame;
    s32 mGrabCeilEnableFallSnapFrame;
    s32 mPoleClimbPreInputSwing;
    f32 mPoleClimbInputRepeatAngle;
    f32 mPoleClimbInputDegreeMove;
    f32 mPoleClimbCatchRange;
    f32 mPoleClimbCatchRangeMin;
    f32 mPoleClimbCatchRangeMax;
    f32 mPoleClimbJointAngleMin;
    f32 mPoleClimbJointAngleMax;
    f32 mPoleClimbJointRangeMin;
    f32 mPoleClimbJointRangeMax;
    f32 mPoleClimbMoveWallDegree;
    f32 mPoleClimbUpMargine;
    f32 mPoleClimbUpSpeed;
    s32 mPoleClimbUpFrame;
    s32 mPoleClimbUpFrameFast;
    s32 mPoleClimbUpFrameSwing;
    f32 mPoleClimbDownSpeed;
    f32 mPoleClimbDownSpeedFast;
    f32 mPoleClimbDownSpeedSwing;
    s32 mPoleClimbDownFrame;
    s32 mPoleClimbDownKeepTime;
    f32 mPoleClimbTurnDist;
    s32 mPoleClimbTurnFrame;
    s32 mPoleClimbTurnStopFrame;
    s32 mPoleTopStartFrame;
    s32 mPoleTopEndFrame;
    f32 mPoleTopTurnSpeed;
    f32 mPoleTopEndUnderOffsetY;
    s32 mGroundSpinFrame;
    f32 mGroundSpinMoveSpeedMax;
    f32 mGroundSpinAccelRate;
    f32 mGroundSpinBrakeRate;
    f32 mSpinJumpGravity;
    f32 mSpinJumpPower;
    f32 mSpinJumpMoveSpeedMax;
    f32 mSpinJumpDownFallInitSpeed;
    f32 mSpinJumpDownFallPower;
    f32 mSpinJumpDownFallSpeedMax;
    f32 mSquatBrakeEndSpeed;
    f32 mSquatAccelRate;
    f32 mSquatBrakeRate;
    f32 mSquatBrakeRateOnSkate;
    f32 mSquatBrakeSideAccel;
    f32 mSquatBrakeSideRate;
    f32 mSquatBrakeSideAccelOnSkate;
    f32 mSquatBrakeSideRateOnSkate;
    f32 mSquatBrakeSideMaxSpeedRate;
    f32 mSquatWalkSpeed;
    f32 mSquatWalkTurnSpeed;
    s32 mSquatWalkTurnFrame;
    f32 mSquatJumpCeilSlideSpeed2D;
    f32 mHipDropSpeed;
    f32 mHipDropGravity;
    f32 mHipDropSpeedMax;
    s32 mHipDropLandCancelFrame;
    f32 mHipDropHeight;
    s32 mHipDropMsgInterval;
    f32 mJumpHipDropPower;
    s32 mJumpHipDropPermitBeginFrame;
    s32 mJumpHipDropPermitEndFrame;
    f32 mHeadSlidingSpeed;
    f32 mHeadSlidingSpeedMin;
    f32 mHeadSlidingBrake;
    f32 mHeadSlidingSideAccel;
    f32 mHeadSlidingJump;
    f32 mHeadSlidingGravityAir;
    f32 mSwimCenterOffset;
    f32 mSwimWallCatchOffset;
    f32 mSwimRisePower;
    f32 mSwimRiseSpeedMax;
    s32 mSwimRiseFrame;
    f32 mSwimGravity;
    f32 mSwimGravityWalk;
    f32 mSwimFallSpeedMax;
    f32 mSwimFloorAccelH;
    f32 mSwimFloorSpeedMaxH;
    f32 mSwimHighAccelH;
    f32 mSwimHighSpeedMaxH;
    f32 mSwimLowAccelH;
    f32 mSwimLowSpeedMaxH;
    f32 mSwimBrakeRateH;
    f32 mSwimWallHitSpeedMinH;
    s32 mSwimHighAccelPermitFrame;
    f32 mSwimFlowFieldBlend;
    f32 mSwimWalkAnimMinRate;
    f32 mSwimWalkAnimMaxRate;
    f32 mSwimWalkMaxSpeed;
    f32 mSwimSpinCapUpPower;
    f32 mSwimSpinCapUpSpeedMax;
    f32 mSwimRotStartAngle;
    f32 mSwimRotFastAngle;
    s32 mSwimRotAccelFrame;
    s32 mSwimRotAccelFrameFast;
    s32 mSwimRotBrakeFrame;
    f32 mSwimRotSpeedChangeStart;
    f32 mSwimRotSpeedForward;
    f32 mSwimRotSpeedMax;
    f32 mSwimSurfaceAccelH;
    f32 mSwimSurfaceSpeedMaxH;
    s32 mSwimSurfaceSpinCapFrame;
    f32 mSwimSurfaceSpinCapSpeedMaxH;
    f32 mSwimSurfaceStartDist;
    f32 mSwimSurfaceEndDist;
    f32 mSwimSurfaceGravity;
    f32 mSwimSurfaceBaseHeight;
    f32 mSwimSurfaceSpring;
    f32 mSwimSurfaceDamper;
    s32 mSwimSurfaceDamperStart;
    s32 mSwimSurfaceDamperFrame;
    f32 mSwimSurfaceEnableJumpHeight;
    s32 mSwimSurfacePreInputJumpFrame;
    f32 mSwimSurfaceMoveSpring;
    f32 mSwimSurfaceMoveDamper;
    f32 mSwimSurfaceMoveBaseHeight;
    f32 mSwimRunSurfaceBaseHeight;
    f32 mSwimRunSurfaceApproachRate;
    f32 mSwimRunSurfaceApproachLimit;
    f32 mSwimRunSurfaceBrakeBorder;
    f32 mSwimRunSurfaceBrakeH;
    f32 mSwimRunSurfaceApproachBorderMax;
    f32 mSwimRunSurfaceApproachBorderMin;
    f32 mSwimRunSurfaceApproachRateMin;
    f32 mSwimFallInSpeed;
    f32 mSwimFallInBrakeH;
    f32 mSwimFallInBrakeV;
    f32 mSwimHeadInBrakeH;
    f32 mSwimHeadInBrakeV;
    f32 mSwimHeadInRisePower;
    f32 mSwimHeadInRiseSpeedMax;
    f32 mSwimHeadInSurfaceHeight;
    s32 mSwimFallInForceSurfaceFrame;
    s32 mSwimFallInvalidJumpFrame;
    f32 mSwimDiveStartSpeed;
    f32 mSwimDiveBrake;
    f32 mSwimDiveEndSpeed;
    s32 mSwimDiveLandCount;
    s32 mSwimDiveLandCancelFrame;
    s32 mSwimDiveNoBrakeFrame;
    s32 mSwimDiveButtonValidFrame;
    s32 mSwimDiveEndFrame;
    f32 mSwimDiveInBrakeH;
    f32 mSwimDiveInBrakeV;
    f32 mSwimDiveInRisePower;
    f32 mSwimDiveInRiseSpeedMax;
    f32 mSwimDiveInSurfaceHeight;
    s32 mSwimDiveInKeepFrame;
    s32 mSwimHeadSlidingFrame;
    s32 mSwimHeadSlidingBrakeFrame;
    f32 mSwimHeadSlidingSpeed;
    f32 mSwimHeadSlidingSpeedEnd;
    f32 mSwimHeadSlidingBrake;
    f32 mSwimHeadSlidingSideAccel;
    f32 mSwimHeadSlidingJump;
    f32 mSwimHeadSlidingGravity;
    s32 mSwimHeadSlidingEndBrakeFrame;
    s32 mSwimHeadSlidingEndSpeedMin;
    f32 mSwimJumpHipDropSpeed;
    f32 mSwimJumpHipDropBrakeV;
    f32 mSwimJumpHipDropBrakeVCeiling;
    f32 mSwimJumpHipDropGravity;
    f32 mSwimJumpHipDropCancelSpeed;
    f32 mSwimJumpHipDropAccelH;
    f32 mSwimJumpHipDropMoveSpeedH;
    f32 mSwimJumpHipDropPopSpeed;
    f32 mSwimJumpHipDropPopJumpAdd;
    f32 mSwimTramplePower;
    f32 mDiveTramplePower;
    s32 mDiveTrampleCancelFrame;
    f32 mDamageSwimPushPower;
    f32 mDamageSwimGravity;
    s32 mDamageSwimCancelFrame;
    f32 mDamageSwimSurfaceGravity;
    f32 mDamageSwimSurfaceHopPower;
    f32 mDamageSwimSurfacePushPower;
    f32 mDamageSwimSurfaceLandSpeed;
    f32 mDamageSwimSurfaceLandBrake;
    f32 mDamageSwimSurfaceLandEndSpeed;
    s32 mDamageSwimSurfaceCancelFrame;
    f32 mDamageSwimBrakeRateGround;
    s32 mOxygenReduceFrame;
    s32 mOxygenNoReduceFrame;
    s32 mOxygenRecoveryFrame;
    s32 mOxygenDamageInterval;
    s32 mIceWaterDamageInterval;
    s32 mIceWaterRecoveryFrame;
    f32 mMoveAnimSpeedMax;
    f32 mAnimFrameRateSpeedMin;
    f32 mRunBorderSpeed;
    f32 mRunBlendRange;
    f32 mDashBorderSpeed;
    f32 mDashBlendRange;
    f32 mDashFastBorderSpeed;
    f32 mDashFastBlendRange;
    f32 mAnimFrameRateSpeedMax;
    f32 mAnimFrameRateRunStart;
    f32 mAnimFrameRateMinRun;
    f32 mAnimFrameRateMaxRun;
    f32 mAnimFrameRateMaxDash;
    f32 mAnimFrameRateMaxDashFast;
    f32 mRunStartPlayFrameScale;
    s32 mRunStartBlendFrame;
    f32 mDamageFireRunAnimRate;
    f32 mRunSkateAnimSpeedOffset;
    f32 mAnimFrameRateRange2D;
    f32 mAnimFrameRateMinRun2D;
    f32 mAnimFrameRateMaxRun2D;
    s32 mIKBlendFrameRun;
    f32 mIKBlendRateRunMin;
    f32 mIKBlendRateRunMax;
    f32 mRollingAnimBorderSpeedMin;
    f32 mRollingAnimBorderSpeedMax;
    f32 mRollingAnimFrameRateMin;
    f32 mRollingAnimFrameRateMax;
    s32 mSwimPaddleAnimInterval;
    s32 mSwimPaddleAnimRateIntervalMax;
    s32 mSwimPaddleAnimRateIntervalMin;
    f32 mSwimPaddleAnimMaxRate;
    f32 mSwimBentForwardMax;
    f32 mSwimBentForwardBlendRate;
    f32 mSwimBentSideMax;
    f32 mSwimBentSpineMax;
    f32 mSwimBentSideBlendRate;
    f32 mSwimBentFrontMax;
    f32 mSwimBentFrontBlendRate;
    f32 mSwimWalkAnimSpeedMax;
    f32 mSwimWalkAnimSpeedMin;
    f32 mSwimWalkAnimFrameRateMax;
    f32 mSwimWalkAnimFrameRateMin;
    f32 mSandSinkBorderMin;
    f32 mSandSinkBorderMax;
    f32 mSandSinkBorderRateMin;
    f32 mSandSinkBorderRateMax;
    f32 mSandSinkFrameRateMin;
    f32 mSandSinkFrameRateMax;
    s32 mLookAtEyeKeepFrame;
    s32 mLookAtEyeKeepFrameInSight;
    s32 mLookAtEyeKeepFrameWait;
    f32 mLookAtEyeDistance;
    f32 mLookAtEyeAngleMinH;
    f32 mLookAtEyeAngleMinInSightH;
    f32 mLookAtEyeAngleMaxH;
    f32 mLookAtEyeAngleMinV;
    f32 mLookAtEyeAngleMinInSightV;
    f32 mLookAtEyeAngleMaxV;
    f32 mTiltEyeBorderStart;
    f32 mTiltEyeBorderEnd;
    f32 mTiltEyeAngleScale;
    f32 mCenterTiltRateMax;
    f32 mNoseChildLocalOffset;
    f32 mNoseStability;
    f32 mNoseFriction;
    f32 mNoseLimitDegree;
    f32 mMustacheChildLocalOffset;
    f32 mMustacheStability;
    f32 mMustacheFriction;
    f32 mMustacheLimitDegree;
    s32 mCapInterpolateFrame;
    f32 mCapChildLocalOffset;
    f32 mCapStability;
    f32 mCapFriction;
    f32 mCapLimitDegree;
    f32 mCapTransStability;
    f32 mCapTransFriction;
    f32 mCapTransLimit;
    s32 mCapManHeroEyesWaitAppearFrame;
    s32 mDeadWipeStartDamage;
    s32 mDeadWipeWaitDamage;
    s32 mDeadWipeStartAbyss;
    s32 mDeadWipeWaitAbyss;
    s32 mDeadWipeStartAbyssWithCapMsg;
    s32 mDeadWipeWaitAbyssWithCapMsg;
    s32 mDeadWipeStartPress;
    s32 mDeadWipeWaitPress;
    s32 mDeadWipeStartSandSink;
    s32 mDeadWipeWaitSandSink;
    s32 mDeadWipeStartNoOxygen;
    s32 mDeadWipeWaitNoOxygen;
    s32 mDeadWipeStartIceWater;
    s32 mDeadWipeWaitIceWater;
    f32 mCoinDashSpeed;
    f32 mCoinDashSpeedLimit;
    f32 mAdditionalSpeedLimit;
};
