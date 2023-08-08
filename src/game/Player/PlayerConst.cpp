#include "game/Player/PlayerConst.h"

PlayerConst::PlayerConst() {
    mGravity = 3.0;
    mFrictionAttack = 0.9;
    mPushPower = 20.0;
    mWaitPoseDegreeMax = 45.0;
    mHillPoseDegreeMax = 45.0;
    mTiltPoseDegreeMax = 20.0;
    mSlerpQuatRate = 0.15;
    mSlerpQuatRateWait = 0.15;
    mSlerpQuatGrav = 0.4;
    mPreInputFrameCapThrow = 10;
    mEnableActionFrameCapCatch = 10;
    mJumpPowerCapCatch = 22.0;
    mJumpGravityCapCatch = 1.3;
    mRunTimeContinuousThrow = 20;
    mRunSpeedMaxContinuousThrow = 16.0;
    mRunAccelFrameContinuousThrow = 60;
    mSeparateCheckHeight = 200.0;
    mSeparateOffsetLerpRate = 0.25;
    mSeparateEnableThrowHeight = 45.0;
    mTall = 160.0;
    mCollisionRadius = 55.0;
    mCollisionRadiusSquat = 45.0;
    mCollisionRadiusStand = 45.0;
    mCollisionSmallStepHeight = 25.0;
    mCollisionResetLimit = 20.0;
    mReflectCeilingPower = 17.0;
    mReflectTossPower = 5.0;
    mReflectUpperPunchScaleH = 0.4;
    mCollisionHitDownAngleH = 55.0;
    mCollisionHitDownEscapeAngleV = 30.0;
    mShadowDropHeightScale = 1.2;
    mShadowDropNormalAdd = 300.0;
    mShadowDropLengthMin = 20.0;
    mShadowDropLengthMax = 4000.0;
    mShadowDropLengthExtend = 10000.0;
    mGravityDamage = 0.95;
    mHopPowerDamage = 12.0;
    mPushPowerDamage = 2.0;
    mDamageCancelFrame = 45;
    mDamageInvalidCount = 240;
    mDamageInvalidCountRecovery = 20;
    mDamageInvalidCountAbyss = 120;
    mNormalMinSpeed2D = 3.0;
    mNormalMaxSpeed2D = 10.0;
    mDashMaxSpeed2D = 17.0;
    mNormalAccelFrame2D = 15;
    mDashAccelFrame2D = 30;
    mNormalDashAnimFrame2D = 15;
    mNormalBrakeFrame2D = 10;
    mStickOnBrakeFrame2D = 60;
    mBrakeTurnStartFrame2D = 5;
    mTurnEndSpeedRate2D = 1.0;
    mJumpPowerMin2DArea = 23.5;
    mJumpPowerMax2DArea = 27.5;
    mJumpPowerMinBorder2D = 12.0;
    mJumpPowerMaxBorder2D = 18.0;
    mGravityMove = 7.0;
    mNormalMaxSpeed = 14.0;
    mNormalMinSpeed = 3.0;
    mNormalAccelFrame = 40;
    mRunAccelAverageScale = 0.5;
    mNormalBrakeFrame = 10;
    mDashJudgeSpeed = 14.5;
    mStickOnBrakeFrame = 120;
    mNormalDashAnimFrame = 15;
    mRunAfterTurnSpeedMax = 17.0;
    mRunAfterTurnScale = 0.5;
    mRunAfterTurnFrame = 30;
    mBrakeTurnStartFrame = 5;
    mBrakeOnSpeedRate = 0.5;
    mBrakeOnCounterBorder = 10;
    mWallPushFrame = 15;
    mRunDeepDownFrame = 10;
    mRunDeepDownMargine = 10;
    mQuickTurnJumpFrame = 20;
    mRoundAccelFrame = 20;
    mRoundBrakeFrame = 20;
    mRoundFastDegree = 45.0;
    mRoundAccelFrameFast = 5;
    mRoundMinDegree = 0.5;
    mRoundBrakeFrameForce = 3;
    mRoundFastDegreeForce = 45.0;
    mRoundLimitDegreeForce = 4.0;
    mRoundLimitDegreeForceFast = 4.0;
    mRoundAccelFrameForceFast = 10;
    mRoundLimitDegreeMin = 6.5;
    mRoundLimitDegree = 8.5;
    mIceAccelFrame = 60;
    mIceBrakeFrame = 120;
    mIceBrakeFrameHigh = 60;
    mIceBrakeFrameWall = 15;
    mIceRoundAccelFrame = 20;
    mIceRoundAccelFrameFast = 1;
    mIceRoundBrakeFrame = 30;
    mIceRoundFastDegree = 45.0;
    mIceRoundMinDegree = 3.0;
    mIceRoundLimitDegree = 25.0;
    mHillAddSpeed = 15.0;
    mHillSubSpeed = 0.0;
    mHillAccelAddFrame = 60;
    mHillAccelSubFrame = 100;
    mHillAccelSubAngleMin = 0.0;
    mHillAccelSubAngleMax = 20.0;
    mStandAngleMin = 60.0;
    mStandAngleMax = 70.0;
    mHillAngleSpeedMin = 21.0;
    mHillAngleSpeedMax = 26.0;
    mSpinCapThrowFrame = 12;
    mSpinCapThrowFrameAir = 8;
    mSpinCapThrowFrameSwim = 12;
    mSpinCapThrowFrameSwing = 7;
    mSpinCapThrowFrameContinuous = 2;
    mSpinAttackFrame = 12;
    mSpinBrakeFrame = 15;
    mSpinAirJumpPower = 6.0;
    mSpinAirSpeedMax = 7.0;
    mGravitySpinAir = 0.3;
    mSlerpQuatRateSpinAir = 0.1;
    mSpinBrakeRate = 0.95;
    mSpinBrakeSideAccel = 0.5;
    mSpinBrakeSideBrakeRate = 0.95;
    mSpinBrakeSideMaxSpeedRate = 0.8;
    mSpinRoundLimitDegree = 5.0;
    mDamageFireJumpPower1st = 20.0;
    mDamageFireJumpPower2nd = 20.0;
    mDamageFireJumpMoveSpeed = 10.0;
    mDamageFireCeilHitSpeed = 1.0;
    mDamageFireGravity = 0.9;
    mDamageFireNoGravityFrame = 20;
    mDamageFireRunTime = 60;
    mDamageFireRunSpeed = 18.0;
    mDamageFireRunBrakeFrame = 20.0;
    mSandSinkDeadTime = 240;
    mSandSinkBrakeHeightH = 100;
    mSandSinkBrakeHeightV = 60;
    mSandSinkHeight = 200.0;
    mSandSinkCapThrow = 30.0;
    mSandSinkBrakeMinH = 0.6;
    mSandSinkBrakeMaxH = 0.1;
    mSandSinkBrakeMinV = 1.0;
    mSandSinkBrakeMaxV = 0.1;
    mSlopeSlideAngleStart = 26.0;
    mSlopeSlideAngleEnd = 10.0;
    mSlopeSlideAccel = 0.3;
    mSlopeSlideBrake = 0.9;
    mSlopeSlideMaxSpeed = 22.0;
    mSlopeSlideSpeedEnd = 3.0;
    mSlopeSlideSideAccel = 0.6;
    mSlopeSlideSideBrake = 0.98;
    mSlopeSlideSideMaxSpeed = 10.0;
    mSlopeTurnDegree = 5.0;
    mSlideInvalidFrame = 15;
    mSlopeForceFrame = 30;
    mSlopeSlideForceSideAccel = 0.2;
    mSlopeSlideForceSideBrake = 0.96;
    mSlopeSlideForceSideMaxSpeed = 3.0;
    mSlopeSlideForceTurnDegree = 15.0;
    mSlopeRollingSpeedStart = 20.0;
    mSlopeRollingSpeedBoost = 30.0;
    mSlopeRollingMaxSpeed = 35.0;
    mSlopeRollingFrameMinBoost = 30;
    mSlopeRollingFrameMin = 45;
    mSlopeRollingStartJumpPower = 12.0;
    mSlopeRollingStartSlideSpeed = 1.0;
    mSlopeRollingAccel = 0.6;
    mSlopeRollingBrake = 0.998;
    mSlopeRollingAgainst = 0.5;
    mSlopeRollingAnglePowerMax = 30.0;
    mSlopeRollingSpeedEnd = 17.0;
    mSlopeRollingSideAccel = 0.4;
    mSlopeRollingSideBrake = 0.985;
    mSlopeRollingSideMaxSpeed = 10.0;
    mSlopeRollingUnRollFrame = 5;
    mSlopeRollingEndBrake = 0.95;
    mSlopeRollingEndBrakeEndSpeed = 10.0;
    mSlopeRollingReStartAccel = 6.0;
    mSlopeRollingReStartMaxAdd = 3.0;
    mSlopeRollingReStartinterval = 15;
    mSlopeRollingReStartSwing = 0;
    mSlopeRollingReStartCharge = 40;
    mSlopeRollingReStartForce = 60;
    mSlopeRollingAccelOnSkate = 0.9;
    mSlopeRollingSideAccelOnSkate = 0.6;
    mSlopeRollingBrakeOnSkate = 0.999;
    mExtendFrame = 10;
    mJumpInertiaRate = 0.7;
    mJumpPowerMin = 17.0;
    mJumpPowerMax = 19.5;
    mJumpGravity = 1.5;
    mJumpBaseSpeedMax = 24.0;
    mJumpMoveSpeedMin = 11.0;
    mJumpMoveSpeedMax = 30.0;
    mJumpAccelFront = 0.5;
    mJumpAccelBack = 1.0;
    mJumpAccelTurn = 0.3;
    mJumpTurnAngleStart = 1.0;
    mJumpTurnAngleLimit = 6.0;
    mJumpTurnAngleFast = 135.0;
    mJumpTurnAngleFastLimit = 25.0;
    mJumpTurnAccelFrame = 20;
    mJumpTurnAccelFrameFast = 1;
    mJumpTurnBrakeFrame = 10;
    mTrampleGravity = 1.75;
    mTrampleJumpPower = 20.0;
    mTrampleHighGravity = 1.0;
    mTrampleHighJumpPower = 25.0;
    mTrampleGravity2D = 1.75;
    mTrampleJumpPower2D = 20.0;
    mTrampleHighGravity2D = 1.0;
    mTrampleHighJumpPower2D = 32.0;
    mTrampleHipDropGravity = 1.5;
    mTrampleHipDropJumpPower = 35.0;
    mTrampleRisingBrakeVelH = 0.3;
    mTrampleJumpCodePower = 57.0;
    mTrampleJumpCodePowerSmall = 35.0;
    mCapLeapFrogJumpGravity = 1.0;
    mCapLeapFrogJumpPower = 32.0;
    mCapLeapFrogJumpPowerAir = 26.0;
    mObjLeapFrogJumpPower = 20.0;
    mObjLeapFrogJumpPowerHigh = 25.0;
    mCapHeadSpringJumpGravity = 1.2;
    mCapHeadSpringJumpPower = 18.0;
    mCapHeadSpringJumpGravityHigh = 1.0;
    mCapHeadSpringJumpPowerHigh = 22.0;
    mCapHeadSpringSpeedMax = 16.0;
    mContinuousJumpPreInputFrame = 5;
    mContinuousJumpCount = 3;
    mContinuousJumpTimer = 10;
    mContinuousJumpPowerMin = 19.5;
    mJumpPowerMax2nd = 21.0;
    mJumpGravity2nd = 1.5;
    mJumpPowerMax3rd = 25.0;
    mJumpGravity3rd = 1.0;
    mSpinFlowerJumpGravity = 0.1;
    mSpinFlowerJumpFallSpeedMax = 8.0;
    mSpinFlowerJumpMovePower = 1.0;
    mSpinFlowerJumpVelMax = 9.0;
    mSpinFlowerJumpStayFrame = 80;
    mSpinFlowerJumpStaySpeedMax = 1.0;
    mSpinFlowerJumpNoInputBrake = 0.95;
    mSpinFlowerJumpDownFallInitSpeed = 15.0;
    mSpinFlowerJumpDownFallPower = 2.0;
    mSpinFlowerJumpDownFallSpeedMax = 30.0;
    mJumpGravityForceRun = 1.0;
    mJumpPowerForceRun = 18.0;
    mCapCatchPopPower = 10.0;
    mCapCatchPopGravity = 0.8;
    mSquatJumpGravity = 1.0;
    mSquatJumpPower = 32.0;
    mSquatJumpBackPower = 5.0;
    mSquatJumpMovePowerFront = 0.2;
    mSquatJumpMovePowerSide = 0.2;
    mSquatJumpMoveSpeedMax = 9.0;
    mTurnJumpGravity = 1.0;
    mTurnJumpPower = 32.0;
    mTurnJumpVelH = 9.0;
    mTurnJumpBrake = 0.5;
    mTurnJumpAccel = 0.25;
    mTurnJumpSideAccel = 0.075;
    mLongJumpAccel = 0.25;
    mLongJumpBrake = 0.5;
    mLongJumpSideAccel = 0.25;
    mLongJumpGravity = 0.48;
    mLongJumpJumpPow = 12.0;
    mLongJumpMovePow = 4.0;
    mLongJumpInitSpeed = 14.0;
    mLongJumpSpeed = 23.0;
    mLongJumpSpeedMin = 2.5;
    mContinuousLongJumpCount = 3;
    mContinuousLongJumpTimer = 15;
    mGravityAir = 1.5;
    mFrictionAir = 0.99;
    mFallSpeedMax = 35.0;
    mLongFallDistance = 3000.0;
    mFallWallScaleVelocity = 0.5;
    mDownFallFrameMin = 5;
    mGravityWallSlide = 0.5;
    mWallHeightLowLimit = 120.0;
    mWallKeepDegree = 60.0;
    mWallKeepFrame = 3;
    mWallJumpGravity = 0.95;
    mWallJumpHSpeed = 8.6;
    mWallJumpPower = 23.0;
    mWallJumpInvalidateInputFrame = 25;
    mWallInhibitAfterPunch = 10;
    mWallFollowAngleH = 20.0;
    mWallFollowAngleV = 30.0;
    mWallCatchDegree = 43.0;
    mWallCatchHeightEdgeTop = 120.0;
    mWallCatchHeightBottom = 150.0;
    mWallCatchKeepDegree = 45.0;
    mWallCatchMoveDegree = 40.0;
    mWallCatchMoveSpeed = 70.0;
    mWallCatchMoveHeightRange = 70.0;
    mWallCatchMoveinterpolate = 10;
    mWallCatchMoveFrame = 10;
    mWallCatchMoveFrameFast = 8;
    mWallCatchMoveFrameSwing = 6;
    mWallCatchInputRepeatAngle = 30.0;
    mWallClimbDegree = 40.0;
    mWallClimbJumpStartFrame = 5;
    mWallClimbJumpEndFrame = 30;
    mWallClimbStartFrame = 20;
    mWallClimbGravity = 15.0;
    mWallFallJumpSpeed = 12.0;
    mWallClimbJumpSpeedV = 20.0;
    mWallClimbJumpSpeedH = 3.0;
    mWallClimbJumpGravity = 1.5;
    mWallClimbJumpInvalidFrame = 12;
    mWallCatchHipLocalOffset = 100.0;
    mWallCatchHipStability = 0.1;
    mWallCatchHipFriction = 0.9;
    mWallCatchHipLimitDegree = 75.0;
    mWallCatchStainAreaOffset = 100.0;
    mGrabCeilRange = 100.0;
    mGrabCeilBodyRadius = 50.0;
    mGrabCeilLeaveSpeedMin = 1.0;
    mGrabCeilLeavePopPower = 3.0;
    mGrabCeilLeavePopGravity = 1.2;
    mGrabCeilSwingStartOffset = 1.0;
    mGrabCeilReverseInputBorder = 6.0;
    mGrabCeilInputPowerBorder = 6.0;
    mGrabCeilSwingWaitEnergy = 6.5;
    mGrabCeilEnableJumpEnergy = 6.5;
    mGrabCeilEnableJumpEnergyMax = 10.0;
    mGrabCeilJumpForceAngle = 135.0;
    mGrabCeilJumpPower = 20.0;
    mGrabCeilJumpMoveMin = 15.0;
    mGrabCeilJumpMoveMax = 15.0;
    mGrabCeilJumpGravity = 1.0;
    mGrabCeilJumpInvalidFrame = 10;
    mGrabCeilEnableNextFrame = 10;
    mGrabCeilEnableFallSnapFrame = 30;
    mPoleClimbPreInputSwing = 15;
    mPoleClimbInputRepeatAngle = 10.0;
    mPoleClimbInputDegreeMove = 50.0;
    mPoleClimbCatchRange = 50.0;
    mPoleClimbCatchRangeMin = 10.0;
    mPoleClimbCatchRangeMax = 100.0;
    mPoleClimbJointAngleMin = -25.0;
    mPoleClimbJointAngleMax = 25.0;
    mPoleClimbJointRangeMin = 15.0;
    mPoleClimbJointRangeMax = 80.0;
    mPoleClimbMoveWallDegree = 5.0;
    mPoleClimbUpMargine = 40.0;
    mPoleClimbUpSpeed = 100.0;
    mPoleClimbUpFrame = 25;
    mPoleClimbUpFrameFast = 17;
    mPoleClimbUpFrameSwing = 12;
    mPoleClimbDownSpeed = 10.0;
    mPoleClimbDownSpeedFast = 15.0;
    mPoleClimbDownSpeedSwing = 20.0;
    mPoleClimbDownFrame = 1;
    mPoleClimbDownKeepTime = 30;
    mPoleClimbTurnDist = 40.0;
    mPoleClimbTurnFrame = 15;
    mPoleClimbTurnStopFrame = 5;
    mPoleTopStartFrame = 10;
    mPoleTopEndFrame = 10;
    mPoleTopTurnSpeed = 4.0;
    mPoleTopEndUnderOffsetY = 60.0;
    mGroundSpinFrame = 90;
    mGroundSpinMoveSpeedMax = 8.0;
    mGroundSpinAccelRate = 0.5;
    mGroundSpinBrakeRate = 0.95;
    mSpinJumpGravity = 0.4;
    mSpinJumpPower = 20.0;
    mSpinJumpMoveSpeedMax = 8.0;
    mSpinJumpDownFallInitSpeed = 35.0;
    mSpinJumpDownFallPower = 1.5;
    mSpinJumpDownFallSpeedMax = 45.0;
    mSquatBrakeEndSpeed = 3.5;
    mSquatAccelRate = 1.2;
    mSquatBrakeRate = 0.95;
    mSquatBrakeRateOnSkate = 0.985;
    mSquatBrakeSideAccel = 0.25;
    mSquatBrakeSideRate = 0.93;
    mSquatBrakeSideAccelOnSkate = 0.1;
    mSquatBrakeSideRateOnSkate = 0.975;
    mSquatBrakeSideMaxSpeedRate = 0.5;
    mSquatWalkSpeed = 3.5;
    mSquatWalkTurnSpeed = 30.0;
    mSquatWalkTurnFrame = 10;
    mSquatJumpCeilSlideSpeed2D = 7.0;
    mHipDropSpeed = 45.0;
    mHipDropGravity = 45.0;
    mHipDropSpeedMax = 45.0;
    mHipDropLandCancelFrame = 24;
    mHipDropHeight = 40.0;
    mHipDropMsginterval = 8;
    mJumpHipDropPower = 40.0;
    mJumpHipDropPermitBeginFrame = 5;
    mJumpHipDropPermitEndFrame = 30;
    mHeadSlidingSpeed = 20.0;
    mHeadSlidingSpeedMin = 2.5;
    mHeadSlidingBrake = 0.5;
    mHeadSlidingSideAccel = 0.125;
    mHeadSlidingJump = 28.0;
    mHeadSlidingGravityAir = 2.0;
    mSwimCenterOffset = 80.0;
    mSwimWallCatchOffset = 100.0;
    mSwimRisePower = 0.6;
    mSwimRiseSpeedMax = 7.5;
    mSwimRiseFrame = 10;
    mSwimGravity = 0.25;
    mSwimGravityWalk = 1.0;
    mSwimFallSpeedMax = 6.5;
    mSwimFloorAccelH = 0.125;
    mSwimFloorSpeedMaxH = 6.5;
    mSwimHighAccelH = 0.25;
    mSwimHighSpeedMaxH = 7.5;
    mSwimLowAccelH = 0.25;
    mSwimLowSpeedMaxH = 6.5;
    mSwimBrakeRateH = 0.975;
    mSwimWallHitSpeedMinH = 3.0;
    mSwimHighAccelPermitFrame = 35;
    mSwimFlowFieldBlend = 0.776;
    mSwimWalkAnimMinRate = 0.2;
    mSwimWalkAnimMaxRate = 1.9;
    mSwimWalkMaxSpeed = 5.0;
    mSwimSpinCapUpPower = 10.0;
    mSwimSpinCapUpSpeedMax = 4.5;
    mSwimRotStartAngle = 1.0;
    mSwimRotFastAngle = 80.0;
    mSwimRotAccelFrame = 20;
    mSwimRotAccelFrameFast = 1;
    mSwimRotBrakeFrame = 30;
    mSwimRotSpeedChangeStart = 3.0;
    mSwimRotSpeedForward = 3.0;
    mSwimRotSpeedMax = 7.5;
    mSwimSurfaceAccelH = 0.25;
    mSwimSurfaceSpeedMaxH = 9.0;
    mSwimSurfaceSpinCapFrame = 45;
    mSwimSurfaceSpinCapSpeedMaxH = 13.0;
    mSwimSurfaceStartDist = 120.0;
    mSwimSurfaceEndDist = 200.0;
    mSwimSurfaceGravity = 0.125;
    mSwimSurfaceBaseHeight = 80.0;
    mSwimSurfaceSpring = 0.05;
    mSwimSurfaceDamper = 0.949;
    mSwimSurfaceDamperStart = 25;
    mSwimSurfaceDamperFrame = 25;
    mSwimSurfaceEnableJumpHeight = 160.0;
    mSwimSurfacePreInputJumpFrame = 8;
    mSwimSurfaceMoveSpring = 0.01;
    mSwimSurfaceMoveDamper = 0.94;
    mSwimSurfaceMoveBaseHeight = 80.0;
    mSwimRunSurfaceBaseHeight = 0.0;
    mSwimRunSurfaceApproachRate = 0.5;
    mSwimRunSurfaceApproachLimit = 5.0;
    mSwimRunSurfaceBrakeBorder = 35.0;
    mSwimRunSurfaceBrakeH = 0.995;
    mSwimRunSurfaceApproachBorderMax = 38.0;
    mSwimRunSurfaceApproachBorderMin = 30.0;
    mSwimRunSurfaceApproachRateMin = 0.06;
    mSwimFallInSpeed = 15.0;
    mSwimFallInBrakeH = 0.95;
    mSwimFallInBrakeV = 0.91;
    mSwimHeadInBrakeH = 0.98;
    mSwimHeadInBrakeV = 0.92;
    mSwimHeadInRisePower = 0.6;
    mSwimHeadInRiseSpeedMax = 10.0;
    mSwimHeadInSurfaceHeight = 50.0;
    mSwimFallInForceSurfaceFrame = 10;
    mSwimFallInvalidJumpFrame = 8;
    mSwimDiveStartSpeed = 26.5;
    mSwimDiveBrake = 0.875;
    mSwimDiveEndSpeed = 1.25;
    mSwimDiveLandCount = 0;
    mSwimDiveLandCancelFrame = 15;
    mSwimDiveNoBrakeFrame = 15;
    mSwimDiveButtonValidFrame = 10;
    mSwimDiveEndFrame = 5;
    mSwimDiveInBrakeH = 0.97;
    mSwimDiveInBrakeV = 0.987;
    mSwimDiveInRisePower = 0.5;
    mSwimDiveInRiseSpeedMax = 3.0;
    mSwimDiveInSurfaceHeight = 50.0;
    mSwimDiveInKeepFrame = 10;
    mSwimHeadSlidingFrame = 15;
    mSwimHeadSlidingBrakeFrame = 45;
    mSwimHeadSlidingSpeed = 15.0;
    mSwimHeadSlidingSpeedEnd = 10.0;
    mSwimHeadSlidingBrake = 0.0;
    mSwimHeadSlidingSideAccel = 0.125;
    mSwimHeadSlidingJump = 0.0;
    mSwimHeadSlidingGravity = 0.0;
    mSwimHeadSlidingEndBrakeFrame = 30;
    mSwimHeadSlidingEndSpeedMin = 5;
    mSwimJumpHipDropSpeed = 27.0;
    mSwimJumpHipDropBrakeV = 0.98;
    mSwimJumpHipDropBrakeVCeiling = 0.5;
    mSwimJumpHipDropGravity = 0.2;
    mSwimJumpHipDropCancelSpeed = 7.5;
    mSwimJumpHipDropAccelH = 0.25;
    mSwimJumpHipDropMoveSpeedH = 3.0;
    mSwimJumpHipDropPopSpeed = 8.0;
    mSwimJumpHipDropPopJumpAdd = 7.0;
    mSwimTramplePower = 8.0;
    mDiveTramplePower = 11.0;
    mDiveTrampleCancelFrame = 20;
    mDamageSwimPushPower = 3.0;
    mDamageSwimGravity = 0.02;
    mDamageSwimCancelFrame = 50;
    mDamageSwimSurfaceGravity = 0.95;
    mDamageSwimSurfaceHopPower = 20.0;
    mDamageSwimSurfacePushPower = 4.0;
    mDamageSwimSurfaceLandSpeed = 5.0;
    mDamageSwimSurfaceLandBrake = 0.05;
    mDamageSwimSurfaceLandEndSpeed = 1.0;
    mDamageSwimSurfaceCancelFrame = 40;
    mDamageSwimBrakeRateGround = 0.95;
    mOxygenReduceFrame = 1380;
    mOxygenNoReduceFrame = 60;
    mOxygenRecoveryFrame = 30;
    mOxygenDamageinterval = 300;
    mIceWaterDamageinterval = 300;
    mIceWaterRecoveryFrame = 70;
    mMoveAnimSpeedMax = 25.0;
    mAnimFrameRateSpeedMin = 6.0;
    mRunBorderSpeed = 8.0;
    mRunBlendRange = 2.0;
    mDashBorderSpeed = 15.0;
    mDashBlendRange = 1.0;
    mDashFastBorderSpeed = 20.0;
    mDashFastBlendRange = 1.0;
    mAnimFrameRateSpeedMax = 26.0;
    mAnimFrameRateRunStart = 2.0;
    mAnimFrameRateMinRun = 1.0;
    mAnimFrameRateMaxRun = 3.5;
    mAnimFrameRateMaxDash = 4.0;
    mAnimFrameRateMaxDashFast = 5.0;
    mRunStartPlayFrameScale = 1.0;
    mRunStartBlendFrame = 5;
    mDamageFireRunAnimRate = 0.3;
    mRunSkateAnimSpeedOffset = 5.0;
    mAnimFrameRateRange2D = 10.0;
    mAnimFrameRateMinRun2D = 1.0;
    mAnimFrameRateMaxRun2D = 2.5;
    mIKBlendFrameRun = 60;
    mIKBlendRateRunMin = 0.85;
    mIKBlendRateRunMax = 0.95;
    mRollingAnimBorderSpeedMin = 5.0;
    mRollingAnimBorderSpeedMax = 35.0;
    mRollingAnimFrameRateMin = 0.2;
    mRollingAnimFrameRateMax = 1.0;
    mSwimPaddleAniminterval = 32;
    mSwimPaddleAnimRateintervalMax = 22;
    mSwimPaddleAnimRateintervalMin = 5;
    mSwimPaddleAnimMaxRate = 3.0;
    mSwimBentForwardMax = 30.0;
    mSwimBentForwardBlendRate = 0.05;
    mSwimBentSideMax = 60.0;
    mSwimBentSpineMax = 40.0;
    mSwimBentSideBlendRate = 0.05;
    mSwimBentFrontMax = 45.0;
    mSwimBentFrontBlendRate = 0.04;
    mSwimWalkAnimSpeedMax = 6.5;
    mSwimWalkAnimSpeedMin = 1.0;
    mSwimWalkAnimFrameRateMax = 1.75;
    mSwimWalkAnimFrameRateMin = 0.5;
    mSandSinkBorderMin = 0.0;
    mSandSinkBorderMax = 0.5;
    mSandSinkBorderRateMin = 0.0;
    mSandSinkBorderRateMax = 0.5;
    mSandSinkFrameRateMin = 4.0;
    mSandSinkFrameRateMax = 0.5;
    mLookAtEyeKeepFrame = 30;
    mLookAtEyeKeepFrameInSight = 0;
    mLookAtEyeKeepFrameWait = 120;
    mLookAtEyeDistance = 500.0;
    mLookAtEyeAngleMinH = 35.0;
    mLookAtEyeAngleMinInSightH = 10.0;
    mLookAtEyeAngleMaxH = 85.0;
    mLookAtEyeAngleMinV = 10.0;
    mLookAtEyeAngleMinInSightV = 10.0;
    mLookAtEyeAngleMaxV = 60.0;
    mTiltEyeBorderStart = 0.9;
    mTiltEyeBorderEnd = 0.25;
    mTiltEyeAngleScale = 0.4;
    mCenterTiltRateMax = 0.0;
    mNoseChildLocalOffset = 50.0;
    mNoseStability = 0.1;
    mNoseFriction = 0.8;
    mNoseLimitDegree = 45.0;
    mMustacheChildLocalOffset = 100.0;
    mMustacheStability = 0.3;
    mMustacheFriction = 0.8;
    mMustacheLimitDegree = 10.0;
    mCapinterpolateFrame = 15;
    mCapChildLocalOffset = 100.0;
    mCapStability = 0.75;
    mCapFriction = 0.2;
    mCapLimitDegree = 15.0;
    mCapTransStability = 0.95;
    mCapTransFriction = 0.25;
    mCapTransLimit = 10.0;
    mCapManHeroEyesWaitAppearFrame = 330;
    mDeadWipeStartDamage = 30;
    mDeadWipeWaitDamage = 96;
    mDeadWipeStartAbyss = 30;
    mDeadWipeWaitAbyss = 30;
    mDeadWipeStartAbyssWithCapMsg = 90;
    mDeadWipeWaitAbyssWithCapMsg = 30;
    mDeadWipeStartPress = 40;
    mDeadWipeWaitPress = 96;
    mDeadWipeStartSandSink = 30;
    mDeadWipeWaitSandSink = 96;
    mDeadWipeStartNoOxygen = 30;
    mDeadWipeWaitNoOxygen = 96;
    mDeadWipeStartIceWater = 30;
    mDeadWipeWaitIceWater = 96;
    mCoinDashSpeed = 6.0;
    mCoinDashSpeedLimit = 6.0;
    mAdditionalSpeedLimit = 30.0;
}
PlayerConst::PlayerConst(const al::ByamlIter& byaml) {
    mGravity = 3.0;
    byaml.tryGetFloatByKey(&mGravity, "Gravity");
    mFrictionAttack = 0.9;
    byaml.tryGetFloatByKey(&mFrictionAttack, "FrictionAttack");
    mPushPower = 20.0;
    byaml.tryGetFloatByKey(&mPushPower, "PushPower");
    mWaitPoseDegreeMax = 45.0;
    byaml.tryGetFloatByKey(&mWaitPoseDegreeMax, "WaitPoseDegreeMax");
    mHillPoseDegreeMax = 45.0;
    byaml.tryGetFloatByKey(&mHillPoseDegreeMax, "HillPoseDegreeMax");
    mTiltPoseDegreeMax = 20.0;
    byaml.tryGetFloatByKey(&mTiltPoseDegreeMax, "TiltPoseDegreeMax");
    mSlerpQuatRate = 0.15;
    byaml.tryGetFloatByKey(&mSlerpQuatRate, "SlerpQuatRate");
    mSlerpQuatRateWait = 0.15;
    byaml.tryGetFloatByKey(&mSlerpQuatRateWait, "SlerpQuatRateWait");
    mSlerpQuatGrav = 0.4;
    byaml.tryGetFloatByKey(&mSlerpQuatGrav, "SlerpQuatGrav");
    mPreInputFrameCapThrow = 10;
    byaml.tryGetIntByKey(&mPreInputFrameCapThrow, "PreInputFrameCapThrow");
    mEnableActionFrameCapCatch = 10;
    byaml.tryGetIntByKey(&mEnableActionFrameCapCatch, "EnableActionFrameCapCatch");
    mJumpPowerCapCatch = 22.0;
    byaml.tryGetFloatByKey(&mJumpPowerCapCatch, "JumpPowerCapCatch");
    mJumpGravityCapCatch = 1.3;
    byaml.tryGetFloatByKey(&mJumpGravityCapCatch, "JumpGravityCapCatch");
    mRunTimeContinuousThrow = 20;
    byaml.tryGetIntByKey(&mRunTimeContinuousThrow, "RunTimeContinuousThrow");
    mRunSpeedMaxContinuousThrow = 16.0;
    byaml.tryGetFloatByKey(&mRunSpeedMaxContinuousThrow, "RunSpeedMaxContinuousThrow");
    mRunAccelFrameContinuousThrow = 60;
    byaml.tryGetIntByKey(&mRunAccelFrameContinuousThrow, "RunAccelFrameContinuousThrow");
    mSeparateCheckHeight = 200.0;
    byaml.tryGetFloatByKey(&mSeparateCheckHeight, "SeparateCheckHeight");
    mSeparateOffsetLerpRate = 0.25;
    byaml.tryGetFloatByKey(&mSeparateOffsetLerpRate, "SeparateOffsetLerpRate");
    mSeparateEnableThrowHeight = 45.0;
    byaml.tryGetFloatByKey(&mSeparateEnableThrowHeight, "SeparateEnableThrowHeight");
    mTall = 160.0;
    byaml.tryGetFloatByKey(&mTall, "Tall");
    mCollisionRadius = 55.0;
    byaml.tryGetFloatByKey(&mCollisionRadius, "CollisionRadius");
    mCollisionRadiusSquat = 45.0;
    byaml.tryGetFloatByKey(&mCollisionRadiusSquat, "CollisionRadiusSquat");
    mCollisionRadiusStand = 45.0;
    byaml.tryGetFloatByKey(&mCollisionRadiusStand, "CollisionRadiusStand");
    mCollisionSmallStepHeight = 25.0;
    byaml.tryGetFloatByKey(&mCollisionSmallStepHeight, "CollisionSmallStepHeight");
    mCollisionResetLimit = 20.0;
    byaml.tryGetFloatByKey(&mCollisionResetLimit, "CollisionResetLimit");
    mReflectCeilingPower = 17.0;
    byaml.tryGetFloatByKey(&mReflectCeilingPower, "ReflectCeilingPower");
    mReflectTossPower = 5.0;
    byaml.tryGetFloatByKey(&mReflectTossPower, "ReflectTossPower");
    mReflectUpperPunchScaleH = 0.4;
    byaml.tryGetFloatByKey(&mReflectUpperPunchScaleH, "ReflectUpperPunchScaleH");
    mCollisionHitDownAngleH = 55.0;
    byaml.tryGetFloatByKey(&mCollisionHitDownAngleH, "CollisionHitDownAngleH");
    mCollisionHitDownEscapeAngleV = 30.0;
    byaml.tryGetFloatByKey(&mCollisionHitDownEscapeAngleV, "CollisionHitDownEscapeAngleV");
    mShadowDropHeightScale = 1.2;
    byaml.tryGetFloatByKey(&mShadowDropHeightScale, "ShadowDropHeightScale");
    mShadowDropNormalAdd = 300.0;
    byaml.tryGetFloatByKey(&mShadowDropNormalAdd, "ShadowDropNormalAdd");
    mShadowDropLengthMin = 20.0;
    byaml.tryGetFloatByKey(&mShadowDropLengthMin, "ShadowDropLengthMin");
    mShadowDropLengthMax = 4000.0;
    byaml.tryGetFloatByKey(&mShadowDropLengthMax, "ShadowDropLengthMax");
    mShadowDropLengthExtend = 10000.0;
    byaml.tryGetFloatByKey(&mShadowDropLengthExtend, "ShadowDropLengthExtend");
    mGravityDamage = 0.95;
    byaml.tryGetFloatByKey(&mGravityDamage, "GravityDamage");
    mHopPowerDamage = 12.0;
    byaml.tryGetFloatByKey(&mHopPowerDamage, "HopPowerDamage");
    mPushPowerDamage = 2.0;
    byaml.tryGetFloatByKey(&mPushPowerDamage, "PushPowerDamage");
    mDamageCancelFrame = 45;
    byaml.tryGetIntByKey(&mDamageCancelFrame, "DamageCancelFrame");
    mDamageInvalidCount = 240;
    byaml.tryGetIntByKey(&mDamageInvalidCount, "DamageInvalidCount");
    mDamageInvalidCountRecovery = 20;
    byaml.tryGetIntByKey(&mDamageInvalidCountRecovery, "DamageInvalidCountRecovery");
    mDamageInvalidCountAbyss = 120;
    byaml.tryGetIntByKey(&mDamageInvalidCountAbyss, "DamageInvalidCountAbyss");
    mNormalMinSpeed2D = 3.0;
    byaml.tryGetFloatByKey(&mNormalMinSpeed2D, "NormalMinSpeed2D");
    mNormalMaxSpeed2D = 10.0;
    byaml.tryGetFloatByKey(&mNormalMaxSpeed2D, "NormalMaxSpeed2D");
    mDashMaxSpeed2D = 17.0;
    byaml.tryGetFloatByKey(&mDashMaxSpeed2D, "DashMaxSpeed2D");
    mNormalAccelFrame2D = 15;
    byaml.tryGetIntByKey(&mNormalAccelFrame2D, "NormalAccelFrame2D");
    mDashAccelFrame2D = 30;
    byaml.tryGetIntByKey(&mDashAccelFrame2D, "DashAccelFrame2D");
    mNormalDashAnimFrame2D = 15;
    byaml.tryGetIntByKey(&mNormalDashAnimFrame2D, "NormalDashAnimFrame2D");
    mNormalBrakeFrame2D = 10;
    byaml.tryGetIntByKey(&mNormalBrakeFrame2D, "NormalBrakeFrame2D");
    mStickOnBrakeFrame2D = 60;
    byaml.tryGetIntByKey(&mStickOnBrakeFrame2D, "StickOnBrakeFrame2D");
    mBrakeTurnStartFrame2D = 5;
    byaml.tryGetIntByKey(&mBrakeTurnStartFrame2D, "BrakeTurnStartFrame2D");
    mTurnEndSpeedRate2D = 1.0;
    byaml.tryGetFloatByKey(&mTurnEndSpeedRate2D, "TurnEndSpeedRate2D");
    mJumpPowerMin2DArea = 23.5;
    byaml.tryGetFloatByKey(&mJumpPowerMin2DArea, "JumpPowerMin2DArea");
    mJumpPowerMax2DArea = 27.5;
    byaml.tryGetFloatByKey(&mJumpPowerMax2DArea, "JumpPowerMax2DArea");
    mJumpPowerMinBorder2D = 12.0;
    byaml.tryGetFloatByKey(&mJumpPowerMinBorder2D, "JumpPowerMinBorder2D");
    mJumpPowerMaxBorder2D = 18.0;
    byaml.tryGetFloatByKey(&mJumpPowerMaxBorder2D, "JumpPowerMaxBorder2D");
    mGravityMove = 7.0;
    byaml.tryGetFloatByKey(&mGravityMove, "GravityMove");
    mNormalMaxSpeed = 14.0;
    byaml.tryGetFloatByKey(&mNormalMaxSpeed, "NormalMaxSpeed");
    mNormalMinSpeed = 3.0;
    byaml.tryGetFloatByKey(&mNormalMinSpeed, "NormalMinSpeed");
    mNormalAccelFrame = 40;
    byaml.tryGetIntByKey(&mNormalAccelFrame, "NormalAccelFrame");
    mRunAccelAverageScale = 0.5;
    byaml.tryGetFloatByKey(&mRunAccelAverageScale, "RunAccelAverageScale");
    mNormalBrakeFrame = 10;
    byaml.tryGetIntByKey(&mNormalBrakeFrame, "NormalBrakeFrame");
    mDashJudgeSpeed = 14.5;
    byaml.tryGetFloatByKey(&mDashJudgeSpeed, "DashJudgeSpeed");
    mStickOnBrakeFrame = 120;
    byaml.tryGetIntByKey(&mStickOnBrakeFrame, "StickOnBrakeFrame");
    mNormalDashAnimFrame = 15;
    byaml.tryGetIntByKey(&mNormalDashAnimFrame, "NormalDashAnimFrame");
    mRunAfterTurnSpeedMax = 17.0;
    byaml.tryGetFloatByKey(&mRunAfterTurnSpeedMax, "RunAfterTurnSpeedMax");
    mRunAfterTurnScale = 0.5;
    byaml.tryGetFloatByKey(&mRunAfterTurnScale, "RunAfterTurnScale");
    mRunAfterTurnFrame = 30;
    byaml.tryGetIntByKey(&mRunAfterTurnFrame, "RunAfterTurnFrame");
    mBrakeTurnStartFrame = 5;
    byaml.tryGetIntByKey(&mBrakeTurnStartFrame, "BrakeTurnStartFrame");
    mBrakeOnSpeedRate = 0.5;
    byaml.tryGetFloatByKey(&mBrakeOnSpeedRate, "BrakeOnSpeedRate");
    mBrakeOnCounterBorder = 10;
    byaml.tryGetIntByKey(&mBrakeOnCounterBorder, "BrakeOnCounterBorder");
    mWallPushFrame = 15;
    byaml.tryGetIntByKey(&mWallPushFrame, "WallPushFrame");
    mRunDeepDownFrame = 10;
    byaml.tryGetIntByKey(&mRunDeepDownFrame, "RunDeepDownFrame");
    mRunDeepDownMargine = 10;
    byaml.tryGetIntByKey(&mRunDeepDownMargine, "RunDeepDownMargine");
    mQuickTurnJumpFrame = 20;
    byaml.tryGetIntByKey(&mQuickTurnJumpFrame, "QuickTurnJumpFrame");
    mRoundAccelFrame = 20;
    byaml.tryGetIntByKey(&mRoundAccelFrame, "RoundAccelFrame");
    mRoundBrakeFrame = 20;
    byaml.tryGetIntByKey(&mRoundBrakeFrame, "RoundBrakeFrame");
    mRoundFastDegree = 45.0;
    byaml.tryGetFloatByKey(&mRoundFastDegree, "RoundFastDegree");
    mRoundAccelFrameFast = 5;
    byaml.tryGetIntByKey(&mRoundAccelFrameFast, "RoundAccelFrameFast");
    mRoundMinDegree = 0.5;
    byaml.tryGetFloatByKey(&mRoundMinDegree, "RoundMinDegree");
    mRoundBrakeFrameForce = 3;
    byaml.tryGetIntByKey(&mRoundBrakeFrameForce, "RoundBrakeFrameForce");
    mRoundFastDegreeForce = 45.0;
    byaml.tryGetFloatByKey(&mRoundFastDegreeForce, "RoundFastDegreeForce");
    mRoundLimitDegreeForce = 4.0;
    byaml.tryGetFloatByKey(&mRoundLimitDegreeForce, "RoundLimitDegreeForce");
    mRoundLimitDegreeForceFast = 4.0;
    byaml.tryGetFloatByKey(&mRoundLimitDegreeForceFast, "RoundLimitDegreeForceFast");
    mRoundAccelFrameForceFast = 10;
    byaml.tryGetIntByKey(&mRoundAccelFrameForceFast, "RoundAccelFrameForceFast");
    mRoundLimitDegreeMin = 6.5;
    byaml.tryGetFloatByKey(&mRoundLimitDegreeMin, "RoundLimitDegreeMin");
    mRoundLimitDegree = 8.5;
    byaml.tryGetFloatByKey(&mRoundLimitDegree, "RoundLimitDegree");
    mIceAccelFrame = 60;
    byaml.tryGetIntByKey(&mIceAccelFrame, "IceAccelFrame");
    mIceBrakeFrame = 120;
    byaml.tryGetIntByKey(&mIceBrakeFrame, "IceBrakeFrame");
    mIceBrakeFrameHigh = 60;
    byaml.tryGetIntByKey(&mIceBrakeFrameHigh, "IceBrakeFrameHigh");
    mIceBrakeFrameWall = 15;
    byaml.tryGetIntByKey(&mIceBrakeFrameWall, "IceBrakeFrameWall");
    mIceRoundAccelFrame = 20;
    byaml.tryGetIntByKey(&mIceRoundAccelFrame, "IceRoundAccelFrame");
    mIceRoundAccelFrameFast = 1;
    byaml.tryGetIntByKey(&mIceRoundAccelFrameFast, "IceRoundAccelFrameFast");
    mIceRoundBrakeFrame = 30;
    byaml.tryGetIntByKey(&mIceRoundBrakeFrame, "IceRoundBrakeFrame");
    mIceRoundFastDegree = 45.0;
    byaml.tryGetFloatByKey(&mIceRoundFastDegree, "IceRoundFastDegree");
    mIceRoundMinDegree = 3.0;
    byaml.tryGetFloatByKey(&mIceRoundMinDegree, "IceRoundMinDegree");
    mIceRoundLimitDegree = 25.0;
    byaml.tryGetFloatByKey(&mIceRoundLimitDegree, "IceRoundLimitDegree");
    mHillAddSpeed = 15.0;
    byaml.tryGetFloatByKey(&mHillAddSpeed, "HillAddSpeed");
    mHillSubSpeed = 0.0;
    byaml.tryGetFloatByKey(&mHillSubSpeed, "HillSubSpeed");
    mHillAccelAddFrame = 60;
    byaml.tryGetIntByKey(&mHillAccelAddFrame, "HillAccelAddFrame");
    mHillAccelSubFrame = 100;
    byaml.tryGetIntByKey(&mHillAccelSubFrame, "HillAccelSubFrame");
    mHillAccelSubAngleMin = 0.0;
    byaml.tryGetFloatByKey(&mHillAccelSubAngleMin, "HillAccelSubAngleMin");
    mHillAccelSubAngleMax = 20.0;
    byaml.tryGetFloatByKey(&mHillAccelSubAngleMax, "HillAccelSubAngleMax");
    mStandAngleMin = 60.0;
    byaml.tryGetFloatByKey(&mStandAngleMin, "StandAngleMin");
    mStandAngleMax = 70.0;
    byaml.tryGetFloatByKey(&mStandAngleMax, "StandAngleMax");
    mHillAngleSpeedMin = 21.0;
    byaml.tryGetFloatByKey(&mHillAngleSpeedMin, "HillAngleSpeedMin");
    mHillAngleSpeedMax = 26.0;
    byaml.tryGetFloatByKey(&mHillAngleSpeedMax, "HillAngleSpeedMax");
    mSpinCapThrowFrame = 12;
    byaml.tryGetIntByKey(&mSpinCapThrowFrame, "SpinCapThrowFrame");
    mSpinCapThrowFrameAir = 8;
    byaml.tryGetIntByKey(&mSpinCapThrowFrameAir, "SpinCapThrowFrameAir");
    mSpinCapThrowFrameSwim = 12;
    byaml.tryGetIntByKey(&mSpinCapThrowFrameSwim, "SpinCapThrowFrameSwim");
    mSpinCapThrowFrameSwing = 7;
    byaml.tryGetIntByKey(&mSpinCapThrowFrameSwing, "SpinCapThrowFrameSwing");
    mSpinCapThrowFrameContinuous = 2;
    byaml.tryGetIntByKey(&mSpinCapThrowFrameContinuous, "SpinCapThrowFrameContinuous");
    mSpinAttackFrame = 12;
    byaml.tryGetIntByKey(&mSpinAttackFrame, "SpinAttackFrame");
    mSpinBrakeFrame = 15;
    byaml.tryGetIntByKey(&mSpinBrakeFrame, "SpinBrakeFrame");
    mSpinAirJumpPower = 6.0;
    byaml.tryGetFloatByKey(&mSpinAirJumpPower, "SpinAirJumpPower");
    mSpinAirSpeedMax = 7.0;
    byaml.tryGetFloatByKey(&mSpinAirSpeedMax, "SpinAirSpeedMax");
    mGravitySpinAir = 0.3;
    byaml.tryGetFloatByKey(&mGravitySpinAir, "GravitySpinAir");
    mSlerpQuatRateSpinAir = 0.1;
    byaml.tryGetFloatByKey(&mSlerpQuatRateSpinAir, "SlerpQuatRateSpinAir");
    mSpinBrakeRate = 0.95;
    byaml.tryGetFloatByKey(&mSpinBrakeRate, "SpinBrakeRate");
    mSpinBrakeSideAccel = 0.5;
    byaml.tryGetFloatByKey(&mSpinBrakeSideAccel, "SpinBrakeSideAccel");
    mSpinBrakeSideBrakeRate = 0.95;
    byaml.tryGetFloatByKey(&mSpinBrakeSideBrakeRate, "SpinBrakeSideBrakeRate");
    mSpinBrakeSideMaxSpeedRate = 0.8;
    byaml.tryGetFloatByKey(&mSpinBrakeSideMaxSpeedRate, "SpinBrakeSideMaxSpeedRate");
    mSpinRoundLimitDegree = 5.0;
    byaml.tryGetFloatByKey(&mSpinRoundLimitDegree, "SpinRoundLimitDegree");
    mDamageFireJumpPower1st = 20.0;
    byaml.tryGetFloatByKey(&mDamageFireJumpPower1st, "DamageFireJumpPower1st");
    mDamageFireJumpPower2nd = 20.0;
    byaml.tryGetFloatByKey(&mDamageFireJumpPower2nd, "DamageFireJumpPower2nd");
    mDamageFireJumpMoveSpeed = 10.0;
    byaml.tryGetFloatByKey(&mDamageFireJumpMoveSpeed, "DamageFireJumpMoveSpeed");
    mDamageFireCeilHitSpeed = 1.0;
    byaml.tryGetFloatByKey(&mDamageFireCeilHitSpeed, "DamageFireCeilHitSpeed");
    mDamageFireGravity = 0.9;
    byaml.tryGetFloatByKey(&mDamageFireGravity, "DamageFireGravity");
    mDamageFireNoGravityFrame = 20;
    byaml.tryGetIntByKey(&mDamageFireNoGravityFrame, "DamageFireNoGravityFrame");
    mDamageFireRunTime = 60;
    byaml.tryGetIntByKey(&mDamageFireRunTime, "DamageFireRunTime");
    mDamageFireRunSpeed = 18.0;
    byaml.tryGetFloatByKey(&mDamageFireRunSpeed, "DamageFireRunSpeed");
    mDamageFireRunBrakeFrame = 20.0;
    byaml.tryGetFloatByKey(&mDamageFireRunBrakeFrame, "DamageFireRunBrakeFrame");
    mSandSinkDeadTime = 240;
    byaml.tryGetIntByKey(&mSandSinkDeadTime, "SandSinkDeadTime");
    mSandSinkBrakeHeightH = 100;
    byaml.tryGetIntByKey(&mSandSinkBrakeHeightH, "SandSinkBrakeHeightH");
    mSandSinkBrakeHeightV = 60;
    byaml.tryGetIntByKey(&mSandSinkBrakeHeightV, "SandSinkBrakeHeightV");
    mSandSinkHeight = 200.0;
    byaml.tryGetFloatByKey(&mSandSinkHeight, "SandSinkHeight");
    mSandSinkCapThrow = 30.0;
    byaml.tryGetFloatByKey(&mSandSinkCapThrow, "SandSinkCapThrow");
    mSandSinkBrakeMinH = 0.6;
    byaml.tryGetFloatByKey(&mSandSinkBrakeMinH, "SandSinkBrakeMinH");
    mSandSinkBrakeMaxH = 0.1;
    byaml.tryGetFloatByKey(&mSandSinkBrakeMaxH, "SandSinkBrakeMaxH");
    mSandSinkBrakeMinV = 1.0;
    byaml.tryGetFloatByKey(&mSandSinkBrakeMinV, "SandSinkBrakeMinV");
    mSandSinkBrakeMaxV = 0.1;
    byaml.tryGetFloatByKey(&mSandSinkBrakeMaxV, "SandSinkBrakeMaxV");
    mSlopeSlideAngleStart = 26.0;
    byaml.tryGetFloatByKey(&mSlopeSlideAngleStart, "SlopeSlideAngleStart");
    mSlopeSlideAngleEnd = 10.0;
    byaml.tryGetFloatByKey(&mSlopeSlideAngleEnd, "SlopeSlideAngleEnd");
    mSlopeSlideAccel = 0.3;
    byaml.tryGetFloatByKey(&mSlopeSlideAccel, "SlopeSlideAccel");
    mSlopeSlideBrake = 0.9;
    byaml.tryGetFloatByKey(&mSlopeSlideBrake, "SlopeSlideBrake");
    mSlopeSlideMaxSpeed = 22.0;
    byaml.tryGetFloatByKey(&mSlopeSlideMaxSpeed, "SlopeSlideMaxSpeed");
    mSlopeSlideSpeedEnd = 3.0;
    byaml.tryGetFloatByKey(&mSlopeSlideSpeedEnd, "SlopeSlideSpeedEnd");
    mSlopeSlideSideAccel = 0.6;
    byaml.tryGetFloatByKey(&mSlopeSlideSideAccel, "SlopeSlideSideAccel");
    mSlopeSlideSideBrake = 0.98;
    byaml.tryGetFloatByKey(&mSlopeSlideSideBrake, "SlopeSlideSideBrake");
    mSlopeSlideSideMaxSpeed = 10.0;
    byaml.tryGetFloatByKey(&mSlopeSlideSideMaxSpeed, "SlopeSlideSideMaxSpeed");
    mSlopeTurnDegree = 5.0;
    byaml.tryGetFloatByKey(&mSlopeTurnDegree, "SlopeTurnDegree");
    mSlideInvalidFrame = 15;
    byaml.tryGetIntByKey(&mSlideInvalidFrame, "SlideInvalidFrame");
    mSlopeForceFrame = 30;
    byaml.tryGetIntByKey(&mSlopeForceFrame, "SlopeForceFrame");
    mSlopeSlideForceSideAccel = 0.2;
    byaml.tryGetFloatByKey(&mSlopeSlideForceSideAccel, "SlopeSlideForceSideAccel");
    mSlopeSlideForceSideBrake = 0.96;
    byaml.tryGetFloatByKey(&mSlopeSlideForceSideBrake, "SlopeSlideForceSideBrake");
    mSlopeSlideForceSideMaxSpeed = 3.0;
    byaml.tryGetFloatByKey(&mSlopeSlideForceSideMaxSpeed, "SlopeSlideForceSideMaxSpeed");
    mSlopeSlideForceTurnDegree = 15.0;
    byaml.tryGetFloatByKey(&mSlopeSlideForceTurnDegree, "SlopeSlideForceTurnDegree");
    mSlopeRollingSpeedStart = 20.0;
    byaml.tryGetFloatByKey(&mSlopeRollingSpeedStart, "SlopeRollingSpeedStart");
    mSlopeRollingSpeedBoost = 30.0;
    byaml.tryGetFloatByKey(&mSlopeRollingSpeedBoost, "SlopeRollingSpeedBoost");
    mSlopeRollingMaxSpeed = 35.0;
    byaml.tryGetFloatByKey(&mSlopeRollingMaxSpeed, "SlopeRollingMaxSpeed");
    mSlopeRollingFrameMinBoost = 30;
    byaml.tryGetIntByKey(&mSlopeRollingFrameMinBoost, "SlopeRollingFrameMinBoost");
    mSlopeRollingFrameMin = 45;
    byaml.tryGetIntByKey(&mSlopeRollingFrameMin, "SlopeRollingFrameMin");
    mSlopeRollingStartJumpPower = 12.0;
    byaml.tryGetFloatByKey(&mSlopeRollingStartJumpPower, "SlopeRollingStartJumpPower");
    mSlopeRollingStartSlideSpeed = 1.0;
    byaml.tryGetFloatByKey(&mSlopeRollingStartSlideSpeed, "SlopeRollingStartSlideSpeed");
    mSlopeRollingAccel = 0.6;
    byaml.tryGetFloatByKey(&mSlopeRollingAccel, "SlopeRollingAccel");
    mSlopeRollingBrake = 0.998;
    byaml.tryGetFloatByKey(&mSlopeRollingBrake, "SlopeRollingBrake");
    mSlopeRollingAgainst = 0.5;
    byaml.tryGetFloatByKey(&mSlopeRollingAgainst, "SlopeRollingAgainst");
    mSlopeRollingAnglePowerMax = 30.0;
    byaml.tryGetFloatByKey(&mSlopeRollingAnglePowerMax, "SlopeRollingAnglePowerMax");
    mSlopeRollingSpeedEnd = 17.0;
    byaml.tryGetFloatByKey(&mSlopeRollingSpeedEnd, "SlopeRollingSpeedEnd");
    mSlopeRollingSideAccel = 0.4;
    byaml.tryGetFloatByKey(&mSlopeRollingSideAccel, "SlopeRollingSideAccel");
    mSlopeRollingSideBrake = 0.985;
    byaml.tryGetFloatByKey(&mSlopeRollingSideBrake, "SlopeRollingSideBrake");
    mSlopeRollingSideMaxSpeed = 10.0;
    byaml.tryGetFloatByKey(&mSlopeRollingSideMaxSpeed, "SlopeRollingSideMaxSpeed");
    mSlopeRollingUnRollFrame = 5;
    byaml.tryGetIntByKey(&mSlopeRollingUnRollFrame, "SlopeRollingUnRollFrame");
    mSlopeRollingEndBrake = 0.95;
    byaml.tryGetFloatByKey(&mSlopeRollingEndBrake, "SlopeRollingEndBrake");
    mSlopeRollingEndBrakeEndSpeed = 10.0;
    byaml.tryGetFloatByKey(&mSlopeRollingEndBrakeEndSpeed, "SlopeRollingEndBrakeEndSpeed");
    mSlopeRollingReStartAccel = 6.0;
    byaml.tryGetFloatByKey(&mSlopeRollingReStartAccel, "SlopeRollingReStartAccel");
    mSlopeRollingReStartMaxAdd = 3.0;
    byaml.tryGetFloatByKey(&mSlopeRollingReStartMaxAdd, "SlopeRollingReStartMaxAdd");
    mSlopeRollingReStartinterval = 15;
    byaml.tryGetIntByKey(&mSlopeRollingReStartinterval, "SlopeRollingReStartinterval");
    mSlopeRollingReStartSwing = 0;
    byaml.tryGetIntByKey(&mSlopeRollingReStartSwing, "SlopeRollingReStartSwing");
    mSlopeRollingReStartCharge = 40;
    byaml.tryGetIntByKey(&mSlopeRollingReStartCharge, "SlopeRollingReStartCharge");
    mSlopeRollingReStartForce = 60;
    byaml.tryGetIntByKey(&mSlopeRollingReStartForce, "SlopeRollingReStartForce");
    mSlopeRollingAccelOnSkate = 0.9;
    byaml.tryGetFloatByKey(&mSlopeRollingAccelOnSkate, "SlopeRollingAccelOnSkate");
    mSlopeRollingSideAccelOnSkate = 0.6;
    byaml.tryGetFloatByKey(&mSlopeRollingSideAccelOnSkate, "SlopeRollingSideAccelOnSkate");
    mSlopeRollingBrakeOnSkate = 0.999;
    byaml.tryGetFloatByKey(&mSlopeRollingBrakeOnSkate, "SlopeRollingBrakeOnSkate");
    mExtendFrame = 10;
    byaml.tryGetIntByKey(&mExtendFrame, "ExtendFrame");
    mJumpInertiaRate = 0.7;
    byaml.tryGetFloatByKey(&mJumpInertiaRate, "JumpInertiaRate");
    mJumpPowerMin = 17.0;
    byaml.tryGetFloatByKey(&mJumpPowerMin, "JumpPowerMin");
    mJumpPowerMax = 19.5;
    byaml.tryGetFloatByKey(&mJumpPowerMax, "JumpPowerMax");
    mJumpGravity = 1.5;
    byaml.tryGetFloatByKey(&mJumpGravity, "JumpGravity");
    mJumpBaseSpeedMax = 24.0;
    byaml.tryGetFloatByKey(&mJumpBaseSpeedMax, "JumpBaseSpeedMax");
    mJumpMoveSpeedMin = 11.0;
    byaml.tryGetFloatByKey(&mJumpMoveSpeedMin, "JumpMoveSpeedMin");
    mJumpMoveSpeedMax = 30.0;
    byaml.tryGetFloatByKey(&mJumpMoveSpeedMax, "JumpMoveSpeedMax");
    mJumpAccelFront = 0.5;
    byaml.tryGetFloatByKey(&mJumpAccelFront, "JumpAccelFront");
    mJumpAccelBack = 1.0;
    byaml.tryGetFloatByKey(&mJumpAccelBack, "JumpAccelBack");
    mJumpAccelTurn = 0.3;
    byaml.tryGetFloatByKey(&mJumpAccelTurn, "JumpAccelTurn");
    mJumpTurnAngleStart = 1.0;
    byaml.tryGetFloatByKey(&mJumpTurnAngleStart, "JumpTurnAngleStart");
    mJumpTurnAngleLimit = 6.0;
    byaml.tryGetFloatByKey(&mJumpTurnAngleLimit, "JumpTurnAngleLimit");
    mJumpTurnAngleFast = 135.0;
    byaml.tryGetFloatByKey(&mJumpTurnAngleFast, "JumpTurnAngleFast");
    mJumpTurnAngleFastLimit = 25.0;
    byaml.tryGetFloatByKey(&mJumpTurnAngleFastLimit, "JumpTurnAngleFastLimit");
    mJumpTurnAccelFrame = 20;
    byaml.tryGetIntByKey(&mJumpTurnAccelFrame, "JumpTurnAccelFrame");
    mJumpTurnAccelFrameFast = 1;
    byaml.tryGetIntByKey(&mJumpTurnAccelFrameFast, "JumpTurnAccelFrameFast");
    mJumpTurnBrakeFrame = 10;
    byaml.tryGetIntByKey(&mJumpTurnBrakeFrame, "JumpTurnBrakeFrame");
    mTrampleGravity = 1.75;
    byaml.tryGetFloatByKey(&mTrampleGravity, "TrampleGravity");
    mTrampleJumpPower = 20.0;
    byaml.tryGetFloatByKey(&mTrampleJumpPower, "TrampleJumpPower");
    mTrampleHighGravity = 1.0;
    byaml.tryGetFloatByKey(&mTrampleHighGravity, "TrampleHighGravity");
    mTrampleHighJumpPower = 25.0;
    byaml.tryGetFloatByKey(&mTrampleHighJumpPower, "TrampleHighJumpPower");
    mTrampleGravity2D = 1.75;
    byaml.tryGetFloatByKey(&mTrampleGravity2D, "TrampleGravity2D");
    mTrampleJumpPower2D = 20.0;
    byaml.tryGetFloatByKey(&mTrampleJumpPower2D, "TrampleJumpPower2D");
    mTrampleHighGravity2D = 1.0;
    byaml.tryGetFloatByKey(&mTrampleHighGravity2D, "TrampleHighGravity2D");
    mTrampleHighJumpPower2D = 32.0;
    byaml.tryGetFloatByKey(&mTrampleHighJumpPower2D, "TrampleHighJumpPower2D");
    mTrampleHipDropGravity = 1.5;
    byaml.tryGetFloatByKey(&mTrampleHipDropGravity, "TrampleHipDropGravity");
    mTrampleHipDropJumpPower = 35.0;
    byaml.tryGetFloatByKey(&mTrampleHipDropJumpPower, "TrampleHipDropJumpPower");
    mTrampleRisingBrakeVelH = 0.3;
    byaml.tryGetFloatByKey(&mTrampleRisingBrakeVelH, "TrampleRisingBrakeVelH");
    mTrampleJumpCodePower = 57.0;
    byaml.tryGetFloatByKey(&mTrampleJumpCodePower, "TrampleJumpCodePower");
    mTrampleJumpCodePowerSmall = 35.0;
    byaml.tryGetFloatByKey(&mTrampleJumpCodePowerSmall, "TrampleJumpCodePowerSmall");
    mCapLeapFrogJumpGravity = 1.0;
    byaml.tryGetFloatByKey(&mCapLeapFrogJumpGravity, "CapLeapFrogJumpGravity");
    mCapLeapFrogJumpPower = 32.0;
    byaml.tryGetFloatByKey(&mCapLeapFrogJumpPower, "CapLeapFrogJumpPower");
    mCapLeapFrogJumpPowerAir = 26.0;
    byaml.tryGetFloatByKey(&mCapLeapFrogJumpPowerAir, "CapLeapFrogJumpPowerAir");
    mObjLeapFrogJumpPower = 20.0;
    byaml.tryGetFloatByKey(&mObjLeapFrogJumpPower, "ObjLeapFrogJumpPower");
    mObjLeapFrogJumpPowerHigh = 25.0;
    byaml.tryGetFloatByKey(&mObjLeapFrogJumpPowerHigh, "ObjLeapFrogJumpPowerHigh");
    mCapHeadSpringJumpGravity = 1.2;
    byaml.tryGetFloatByKey(&mCapHeadSpringJumpGravity, "CapHeadSpringJumpGravity");
    mCapHeadSpringJumpPower = 18.0;
    byaml.tryGetFloatByKey(&mCapHeadSpringJumpPower, "CapHeadSpringJumpPower");
    mCapHeadSpringJumpGravityHigh = 1.0;
    byaml.tryGetFloatByKey(&mCapHeadSpringJumpGravityHigh, "CapHeadSpringJumpGravityHigh");
    mCapHeadSpringJumpPowerHigh = 22.0;
    byaml.tryGetFloatByKey(&mCapHeadSpringJumpPowerHigh, "CapHeadSpringJumpPowerHigh");
    mCapHeadSpringSpeedMax = 16.0;
    byaml.tryGetFloatByKey(&mCapHeadSpringSpeedMax, "CapHeadSpringSpeedMax");
    mContinuousJumpPreInputFrame = 5;
    byaml.tryGetIntByKey(&mContinuousJumpPreInputFrame, "ContinuousJumpPreInputFrame");
    mContinuousJumpCount = 3;
    byaml.tryGetIntByKey(&mContinuousJumpCount, "ContinuousJumpCount");
    mContinuousJumpTimer = 10;
    byaml.tryGetIntByKey(&mContinuousJumpTimer, "ContinuousJumpTimer");
    mContinuousJumpPowerMin = 19.5;
    byaml.tryGetFloatByKey(&mContinuousJumpPowerMin, "ContinuousJumpPowerMin");
    mJumpPowerMax2nd = 21.0;
    byaml.tryGetFloatByKey(&mJumpPowerMax2nd, "JumpPowerMax2nd");
    mJumpGravity2nd = 1.5;
    byaml.tryGetFloatByKey(&mJumpGravity2nd, "JumpGravity2nd");
    mJumpPowerMax3rd = 25.0;
    byaml.tryGetFloatByKey(&mJumpPowerMax3rd, "JumpPowerMax3rd");
    mJumpGravity3rd = 1.0;
    byaml.tryGetFloatByKey(&mJumpGravity3rd, "JumpGravity3rd");
    mSpinFlowerJumpGravity = 0.1;
    byaml.tryGetFloatByKey(&mSpinFlowerJumpGravity, "SpinFlowerJumpGravity");
    mSpinFlowerJumpFallSpeedMax = 8.0;
    byaml.tryGetFloatByKey(&mSpinFlowerJumpFallSpeedMax, "SpinFlowerJumpFallSpeedMax");
    mSpinFlowerJumpMovePower = 1.0;
    byaml.tryGetFloatByKey(&mSpinFlowerJumpMovePower, "SpinFlowerJumpMovePower");
    mSpinFlowerJumpVelMax = 9.0;
    byaml.tryGetFloatByKey(&mSpinFlowerJumpVelMax, "SpinFlowerJumpVelMax");
    mSpinFlowerJumpStayFrame = 80;
    byaml.tryGetIntByKey(&mSpinFlowerJumpStayFrame, "SpinFlowerJumpStayFrame");
    mSpinFlowerJumpStaySpeedMax = 1.0;
    byaml.tryGetFloatByKey(&mSpinFlowerJumpStaySpeedMax, "SpinFlowerJumpStaySpeedMax");
    mSpinFlowerJumpNoInputBrake = 0.95;
    byaml.tryGetFloatByKey(&mSpinFlowerJumpNoInputBrake, "SpinFlowerJumpNoInputBrake");
    mSpinFlowerJumpDownFallInitSpeed = 15.0;
    byaml.tryGetFloatByKey(&mSpinFlowerJumpDownFallInitSpeed, "SpinFlowerJumpDownFallInitSpeed");
    mSpinFlowerJumpDownFallPower = 2.0;
    byaml.tryGetFloatByKey(&mSpinFlowerJumpDownFallPower, "SpinFlowerJumpDownFallPower");
    mSpinFlowerJumpDownFallSpeedMax = 30.0;
    byaml.tryGetFloatByKey(&mSpinFlowerJumpDownFallSpeedMax, "SpinFlowerJumpDownFallSpeedMax");
    mJumpGravityForceRun = 1.0;
    byaml.tryGetFloatByKey(&mJumpGravityForceRun, "JumpGravityForceRun");
    mJumpPowerForceRun = 18.0;
    byaml.tryGetFloatByKey(&mJumpPowerForceRun, "JumpPowerForceRun");
    mCapCatchPopPower = 10.0;
    byaml.tryGetFloatByKey(&mCapCatchPopPower, "CapCatchPopPower");
    mCapCatchPopGravity = 0.8;
    byaml.tryGetFloatByKey(&mCapCatchPopGravity, "CapCatchPopGravity");
    mSquatJumpGravity = 1.0;
    byaml.tryGetFloatByKey(&mSquatJumpGravity, "SquatJumpGravity");
    mSquatJumpPower = 32.0;
    byaml.tryGetFloatByKey(&mSquatJumpPower, "SquatJumpPower");
    mSquatJumpBackPower = 5.0;
    byaml.tryGetFloatByKey(&mSquatJumpBackPower, "SquatJumpBackPower");
    mSquatJumpMovePowerFront = 0.2;
    byaml.tryGetFloatByKey(&mSquatJumpMovePowerFront, "SquatJumpMovePowerFront");
    mSquatJumpMovePowerSide = 0.2;
    byaml.tryGetFloatByKey(&mSquatJumpMovePowerSide, "SquatJumpMovePowerSide");
    mSquatJumpMoveSpeedMax = 9.0;
    byaml.tryGetFloatByKey(&mSquatJumpMoveSpeedMax, "SquatJumpMoveSpeedMax");
    mTurnJumpGravity = 1.0;
    byaml.tryGetFloatByKey(&mTurnJumpGravity, "TurnJumpGravity");
    mTurnJumpPower = 32.0;
    byaml.tryGetFloatByKey(&mTurnJumpPower, "TurnJumpPower");
    mTurnJumpVelH = 9.0;
    byaml.tryGetFloatByKey(&mTurnJumpVelH, "TurnJumpVelH");
    mTurnJumpBrake = 0.5;
    byaml.tryGetFloatByKey(&mTurnJumpBrake, "TurnJumpBrake");
    mTurnJumpAccel = 0.25;
    byaml.tryGetFloatByKey(&mTurnJumpAccel, "TurnJumpAccel");
    mTurnJumpSideAccel = 0.075;
    byaml.tryGetFloatByKey(&mTurnJumpSideAccel, "TurnJumpSideAccel");
    mLongJumpAccel = 0.25;
    byaml.tryGetFloatByKey(&mLongJumpAccel, "LongJumpAccel");
    mLongJumpBrake = 0.5;
    byaml.tryGetFloatByKey(&mLongJumpBrake, "LongJumpBrake");
    mLongJumpSideAccel = 0.25;
    byaml.tryGetFloatByKey(&mLongJumpSideAccel, "LongJumpSideAccel");
    mLongJumpGravity = 0.48;
    byaml.tryGetFloatByKey(&mLongJumpGravity, "LongJumpGravity");
    mLongJumpJumpPow = 12.0;
    byaml.tryGetFloatByKey(&mLongJumpJumpPow, "LongJumpJumpPow");
    mLongJumpMovePow = 4.0;
    byaml.tryGetFloatByKey(&mLongJumpMovePow, "LongJumpMovePow");
    mLongJumpInitSpeed = 14.0;
    byaml.tryGetFloatByKey(&mLongJumpInitSpeed, "LongJumpInitSpeed");
    mLongJumpSpeed = 23.0;
    byaml.tryGetFloatByKey(&mLongJumpSpeed, "LongJumpSpeed");
    mLongJumpSpeedMin = 2.5;
    byaml.tryGetFloatByKey(&mLongJumpSpeedMin, "LongJumpSpeedMin");
    mContinuousLongJumpCount = 3;
    byaml.tryGetIntByKey(&mContinuousLongJumpCount, "ContinuousLongJumpCount");
    mContinuousLongJumpTimer = 15;
    byaml.tryGetIntByKey(&mContinuousLongJumpTimer, "ContinuousLongJumpTimer");
    mGravityAir = 1.5;
    byaml.tryGetFloatByKey(&mGravityAir, "GravityAir");
    mFrictionAir = 0.99;
    byaml.tryGetFloatByKey(&mFrictionAir, "FrictionAir");
    mFallSpeedMax = 35.0;
    byaml.tryGetFloatByKey(&mFallSpeedMax, "FallSpeedMax");
    mLongFallDistance = 3000.0;
    byaml.tryGetFloatByKey(&mLongFallDistance, "LongFallDistance");
    mFallWallScaleVelocity = 0.5;
    byaml.tryGetFloatByKey(&mFallWallScaleVelocity, "FallWallScaleVelocity");
    mDownFallFrameMin = 5;
    byaml.tryGetIntByKey(&mDownFallFrameMin, "DownFallFrameMin");
    mGravityWallSlide = 0.5;
    byaml.tryGetFloatByKey(&mGravityWallSlide, "GravityWallSlide");
    mWallHeightLowLimit = 120.0;
    byaml.tryGetFloatByKey(&mWallHeightLowLimit, "WallHeightLowLimit");
    mWallKeepDegree = 60.0;
    byaml.tryGetFloatByKey(&mWallKeepDegree, "WallKeepDegree");
    mWallKeepFrame = 3;
    byaml.tryGetIntByKey(&mWallKeepFrame, "WallKeepFrame");
    mWallJumpGravity = 0.95;
    byaml.tryGetFloatByKey(&mWallJumpGravity, "WallJumpGravity");
    mWallJumpHSpeed = 8.6;
    byaml.tryGetFloatByKey(&mWallJumpHSpeed, "WallJumpHSpeed");
    mWallJumpPower = 23.0;
    byaml.tryGetFloatByKey(&mWallJumpPower, "WallJumpPower");
    mWallJumpInvalidateInputFrame = 25;
    byaml.tryGetIntByKey(&mWallJumpInvalidateInputFrame, "WallJumpInvalidateInputFrame");
    mWallInhibitAfterPunch = 10;
    byaml.tryGetIntByKey(&mWallInhibitAfterPunch, "WallInhibitAfterPunch");
    mWallFollowAngleH = 20.0;
    byaml.tryGetFloatByKey(&mWallFollowAngleH, "WallFollowAngleH");
    mWallFollowAngleV = 30.0;
    byaml.tryGetFloatByKey(&mWallFollowAngleV, "WallFollowAngleV");
    mWallCatchDegree = 43.0;
    byaml.tryGetFloatByKey(&mWallCatchDegree, "WallCatchDegree");
    mWallCatchHeightEdgeTop = 120.0;
    byaml.tryGetFloatByKey(&mWallCatchHeightEdgeTop, "WallCatchHeightEdgeTop");
    mWallCatchHeightBottom = 150.0;
    byaml.tryGetFloatByKey(&mWallCatchHeightBottom, "WallCatchHeightBottom");
    mWallCatchKeepDegree = 45.0;
    byaml.tryGetFloatByKey(&mWallCatchKeepDegree, "WallCatchKeepDegree");
    mWallCatchMoveDegree = 40.0;
    byaml.tryGetFloatByKey(&mWallCatchMoveDegree, "WallCatchMoveDegree");
    mWallCatchMoveSpeed = 70.0;
    byaml.tryGetFloatByKey(&mWallCatchMoveSpeed, "WallCatchMoveSpeed");
    mWallCatchMoveHeightRange = 70.0;
    byaml.tryGetFloatByKey(&mWallCatchMoveHeightRange, "WallCatchMoveHeightRange");
    mWallCatchMoveinterpolate = 10;
    byaml.tryGetIntByKey(&mWallCatchMoveinterpolate, "WallCatchMoveinterpolate");
    mWallCatchMoveFrame = 10;
    byaml.tryGetIntByKey(&mWallCatchMoveFrame, "WallCatchMoveFrame");
    mWallCatchMoveFrameFast = 8;
    byaml.tryGetIntByKey(&mWallCatchMoveFrameFast, "WallCatchMoveFrameFast");
    mWallCatchMoveFrameSwing = 6;
    byaml.tryGetIntByKey(&mWallCatchMoveFrameSwing, "WallCatchMoveFrameSwing");
    mWallCatchInputRepeatAngle = 30.0;
    byaml.tryGetFloatByKey(&mWallCatchInputRepeatAngle, "WallCatchInputRepeatAngle");
    mWallClimbDegree = 40.0;
    byaml.tryGetFloatByKey(&mWallClimbDegree, "WallClimbDegree");
    mWallClimbJumpStartFrame = 5;
    byaml.tryGetIntByKey(&mWallClimbJumpStartFrame, "WallClimbJumpStartFrame");
    mWallClimbJumpEndFrame = 30;
    byaml.tryGetIntByKey(&mWallClimbJumpEndFrame, "WallClimbJumpEndFrame");
    mWallClimbStartFrame = 20;
    byaml.tryGetIntByKey(&mWallClimbStartFrame, "WallClimbStartFrame");
    mWallClimbGravity = 15.0;
    byaml.tryGetFloatByKey(&mWallClimbGravity, "WallClimbGravity");
    mWallFallJumpSpeed = 12.0;
    byaml.tryGetFloatByKey(&mWallFallJumpSpeed, "WallFallJumpSpeed");
    mWallClimbJumpSpeedV = 20.0;
    byaml.tryGetFloatByKey(&mWallClimbJumpSpeedV, "WallClimbJumpSpeedV");
    mWallClimbJumpSpeedH = 3.0;
    byaml.tryGetFloatByKey(&mWallClimbJumpSpeedH, "WallClimbJumpSpeedH");
    mWallClimbJumpGravity = 1.5;
    byaml.tryGetFloatByKey(&mWallClimbJumpGravity, "WallClimbJumpGravity");
    mWallClimbJumpInvalidFrame = 12;
    byaml.tryGetIntByKey(&mWallClimbJumpInvalidFrame, "WallClimbJumpInvalidFrame");
    mWallCatchHipLocalOffset = 100.0;
    byaml.tryGetFloatByKey(&mWallCatchHipLocalOffset, "WallCatchHipLocalOffset");
    mWallCatchHipStability = 0.1;
    byaml.tryGetFloatByKey(&mWallCatchHipStability, "WallCatchHipStability");
    mWallCatchHipFriction = 0.9;
    byaml.tryGetFloatByKey(&mWallCatchHipFriction, "WallCatchHipFriction");
    mWallCatchHipLimitDegree = 75.0;
    byaml.tryGetFloatByKey(&mWallCatchHipLimitDegree, "WallCatchHipLimitDegree");
    mWallCatchStainAreaOffset = 100.0;
    byaml.tryGetFloatByKey(&mWallCatchStainAreaOffset, "WallCatchStainAreaOffset");
    mGrabCeilRange = 100.0;
    byaml.tryGetFloatByKey(&mGrabCeilRange, "GrabCeilRange");
    mGrabCeilBodyRadius = 50.0;
    byaml.tryGetFloatByKey(&mGrabCeilBodyRadius, "GrabCeilBodyRadius");
    mGrabCeilLeaveSpeedMin = 1.0;
    byaml.tryGetFloatByKey(&mGrabCeilLeaveSpeedMin, "GrabCeilLeaveSpeedMin");
    mGrabCeilLeavePopPower = 3.0;
    byaml.tryGetFloatByKey(&mGrabCeilLeavePopPower, "GrabCeilLeavePopPower");
    mGrabCeilLeavePopGravity = 1.2;
    byaml.tryGetFloatByKey(&mGrabCeilLeavePopGravity, "GrabCeilLeavePopGravity");
    mGrabCeilSwingStartOffset = 1.0;
    byaml.tryGetFloatByKey(&mGrabCeilSwingStartOffset, "GrabCeilSwingStartOffset");
    mGrabCeilReverseInputBorder = 6.0;
    byaml.tryGetFloatByKey(&mGrabCeilReverseInputBorder, "GrabCeilReverseInputBorder");
    mGrabCeilInputPowerBorder = 6.0;
    byaml.tryGetFloatByKey(&mGrabCeilInputPowerBorder, "GrabCeilInputPowerBorder");
    mGrabCeilSwingWaitEnergy = 6.5;
    byaml.tryGetFloatByKey(&mGrabCeilSwingWaitEnergy, "GrabCeilSwingWaitEnergy");
    mGrabCeilEnableJumpEnergy = 6.5;
    byaml.tryGetFloatByKey(&mGrabCeilEnableJumpEnergy, "GrabCeilEnableJumpEnergy");
    mGrabCeilEnableJumpEnergyMax = 10.0;
    byaml.tryGetFloatByKey(&mGrabCeilEnableJumpEnergyMax, "GrabCeilEnableJumpEnergyMax");
    mGrabCeilJumpForceAngle = 135.0;
    byaml.tryGetFloatByKey(&mGrabCeilJumpForceAngle, "GrabCeilJumpForceAngle");
    mGrabCeilJumpPower = 20.0;
    byaml.tryGetFloatByKey(&mGrabCeilJumpPower, "GrabCeilJumpPower");
    mGrabCeilJumpMoveMin = 15.0;
    byaml.tryGetFloatByKey(&mGrabCeilJumpMoveMin, "GrabCeilJumpMoveMin");
    mGrabCeilJumpMoveMax = 15.0;
    byaml.tryGetFloatByKey(&mGrabCeilJumpMoveMax, "GrabCeilJumpMoveMax");
    mGrabCeilJumpGravity = 1.0;
    byaml.tryGetFloatByKey(&mGrabCeilJumpGravity, "GrabCeilJumpGravity");
    mGrabCeilJumpInvalidFrame = 10;
    byaml.tryGetIntByKey(&mGrabCeilJumpInvalidFrame, "GrabCeilJumpInvalidFrame");
    mGrabCeilEnableNextFrame = 10;
    byaml.tryGetIntByKey(&mGrabCeilEnableNextFrame, "GrabCeilEnableNextFrame");
    mGrabCeilEnableFallSnapFrame = 30;
    byaml.tryGetIntByKey(&mGrabCeilEnableFallSnapFrame, "GrabCeilEnableFallSnapFrame");
    mPoleClimbPreInputSwing = 15;
    byaml.tryGetIntByKey(&mPoleClimbPreInputSwing, "PoleClimbPreInputSwing");
    mPoleClimbInputRepeatAngle = 10.0;
    byaml.tryGetFloatByKey(&mPoleClimbInputRepeatAngle, "PoleClimbInputRepeatAngle");
    mPoleClimbInputDegreeMove = 50.0;
    byaml.tryGetFloatByKey(&mPoleClimbInputDegreeMove, "PoleClimbInputDegreeMove");
    mPoleClimbCatchRange = 50.0;
    byaml.tryGetFloatByKey(&mPoleClimbCatchRange, "PoleClimbCatchRange");
    mPoleClimbCatchRangeMin = 10.0;
    byaml.tryGetFloatByKey(&mPoleClimbCatchRangeMin, "PoleClimbCatchRangeMin");
    mPoleClimbCatchRangeMax = 100.0;
    byaml.tryGetFloatByKey(&mPoleClimbCatchRangeMax, "PoleClimbCatchRangeMax");
    mPoleClimbJointAngleMin = -25.0;
    byaml.tryGetFloatByKey(&mPoleClimbJointAngleMin, "PoleClimbJointAngleMin");
    mPoleClimbJointAngleMax = 25.0;
    byaml.tryGetFloatByKey(&mPoleClimbJointAngleMax, "PoleClimbJointAngleMax");
    mPoleClimbJointRangeMin = 15.0;
    byaml.tryGetFloatByKey(&mPoleClimbJointRangeMin, "PoleClimbJointRangeMin");
    mPoleClimbJointRangeMax = 80.0;
    byaml.tryGetFloatByKey(&mPoleClimbJointRangeMax, "PoleClimbJointRangeMax");
    mPoleClimbMoveWallDegree = 5.0;
    byaml.tryGetFloatByKey(&mPoleClimbMoveWallDegree, "PoleClimbMoveWallDegree");
    mPoleClimbUpMargine = 40.0;
    byaml.tryGetFloatByKey(&mPoleClimbUpMargine, "PoleClimbUpMargine");
    mPoleClimbUpSpeed = 100.0;
    byaml.tryGetFloatByKey(&mPoleClimbUpSpeed, "PoleClimbUpSpeed");
    mPoleClimbUpFrame = 25;
    byaml.tryGetIntByKey(&mPoleClimbUpFrame, "PoleClimbUpFrame");
    mPoleClimbUpFrameFast = 17;
    byaml.tryGetIntByKey(&mPoleClimbUpFrameFast, "PoleClimbUpFrameFast");
    mPoleClimbUpFrameSwing = 12;
    byaml.tryGetIntByKey(&mPoleClimbUpFrameSwing, "PoleClimbUpFrameSwing");
    mPoleClimbDownSpeed = 10.0;
    byaml.tryGetFloatByKey(&mPoleClimbDownSpeed, "PoleClimbDownSpeed");
    mPoleClimbDownSpeedFast = 15.0;
    byaml.tryGetFloatByKey(&mPoleClimbDownSpeedFast, "PoleClimbDownSpeedFast");
    mPoleClimbDownSpeedSwing = 20.0;
    byaml.tryGetFloatByKey(&mPoleClimbDownSpeedSwing, "PoleClimbDownSpeedSwing");
    mPoleClimbDownFrame = 1;
    byaml.tryGetIntByKey(&mPoleClimbDownFrame, "PoleClimbDownFrame");
    mPoleClimbDownKeepTime = 30;
    byaml.tryGetIntByKey(&mPoleClimbDownKeepTime, "PoleClimbDownKeepTime");
    mPoleClimbTurnDist = 40.0;
    byaml.tryGetFloatByKey(&mPoleClimbTurnDist, "PoleClimbTurnDist");
    mPoleClimbTurnFrame = 15;
    byaml.tryGetIntByKey(&mPoleClimbTurnFrame, "PoleClimbTurnFrame");
    mPoleClimbTurnStopFrame = 5;
    byaml.tryGetIntByKey(&mPoleClimbTurnStopFrame, "PoleClimbTurnStopFrame");
    mPoleTopStartFrame = 10;
    byaml.tryGetIntByKey(&mPoleTopStartFrame, "PoleTopStartFrame");
    mPoleTopEndFrame = 10;
    byaml.tryGetIntByKey(&mPoleTopEndFrame, "PoleTopEndFrame");
    mPoleTopTurnSpeed = 4.0;
    byaml.tryGetFloatByKey(&mPoleTopTurnSpeed, "PoleTopTurnSpeed");
    mPoleTopEndUnderOffsetY = 60.0;
    byaml.tryGetFloatByKey(&mPoleTopEndUnderOffsetY, "PoleTopEndUnderOffsetY");
    mGroundSpinFrame = 90;
    byaml.tryGetIntByKey(&mGroundSpinFrame, "GroundSpinFrame");
    mGroundSpinMoveSpeedMax = 8.0;
    byaml.tryGetFloatByKey(&mGroundSpinMoveSpeedMax, "GroundSpinMoveSpeedMax");
    mGroundSpinAccelRate = 0.5;
    byaml.tryGetFloatByKey(&mGroundSpinAccelRate, "GroundSpinAccelRate");
    mGroundSpinBrakeRate = 0.95;
    byaml.tryGetFloatByKey(&mGroundSpinBrakeRate, "GroundSpinBrakeRate");
    mSpinJumpGravity = 0.4;
    byaml.tryGetFloatByKey(&mSpinJumpGravity, "SpinJumpGravity");
    mSpinJumpPower = 20.0;
    byaml.tryGetFloatByKey(&mSpinJumpPower, "SpinJumpPower");
    mSpinJumpMoveSpeedMax = 8.0;
    byaml.tryGetFloatByKey(&mSpinJumpMoveSpeedMax, "SpinJumpMoveSpeedMax");
    mSpinJumpDownFallInitSpeed = 35.0;
    byaml.tryGetFloatByKey(&mSpinJumpDownFallInitSpeed, "SpinJumpDownFallInitSpeed");
    mSpinJumpDownFallPower = 1.5;
    byaml.tryGetFloatByKey(&mSpinJumpDownFallPower, "SpinJumpDownFallPower");
    mSpinJumpDownFallSpeedMax = 45.0;
    byaml.tryGetFloatByKey(&mSpinJumpDownFallSpeedMax, "SpinJumpDownFallSpeedMax");
    mSquatBrakeEndSpeed = 3.5;
    byaml.tryGetFloatByKey(&mSquatBrakeEndSpeed, "SquatBrakeEndSpeed");
    mSquatAccelRate = 1.2;
    byaml.tryGetFloatByKey(&mSquatAccelRate, "SquatAccelRate");
    mSquatBrakeRate = 0.95;
    byaml.tryGetFloatByKey(&mSquatBrakeRate, "SquatBrakeRate");
    mSquatBrakeRateOnSkate = 0.985;
    byaml.tryGetFloatByKey(&mSquatBrakeRateOnSkate, "SquatBrakeRateOnSkate");
    mSquatBrakeSideAccel = 0.25;
    byaml.tryGetFloatByKey(&mSquatBrakeSideAccel, "SquatBrakeSideAccel");
    mSquatBrakeSideRate = 0.93;
    byaml.tryGetFloatByKey(&mSquatBrakeSideRate, "SquatBrakeSideRate");
    mSquatBrakeSideAccelOnSkate = 0.1;
    byaml.tryGetFloatByKey(&mSquatBrakeSideAccelOnSkate, "SquatBrakeSideAccelOnSkate");
    mSquatBrakeSideRateOnSkate = 0.975;
    byaml.tryGetFloatByKey(&mSquatBrakeSideRateOnSkate, "SquatBrakeSideRateOnSkate");
    mSquatBrakeSideMaxSpeedRate = 0.5;
    byaml.tryGetFloatByKey(&mSquatBrakeSideMaxSpeedRate, "SquatBrakeSideMaxSpeedRate");
    mSquatWalkSpeed = 3.5;
    byaml.tryGetFloatByKey(&mSquatWalkSpeed, "SquatWalkSpeed");
    mSquatWalkTurnSpeed = 30.0;
    byaml.tryGetFloatByKey(&mSquatWalkTurnSpeed, "SquatWalkTurnSpeed");
    mSquatWalkTurnFrame = 10;
    byaml.tryGetIntByKey(&mSquatWalkTurnFrame, "SquatWalkTurnFrame");
    mSquatJumpCeilSlideSpeed2D = 7.0;
    byaml.tryGetFloatByKey(&mSquatJumpCeilSlideSpeed2D, "SquatJumpCeilSlideSpeed2D");
    mHipDropSpeed = 45.0;
    byaml.tryGetFloatByKey(&mHipDropSpeed, "HipDropSpeed");
    mHipDropGravity = 45.0;
    byaml.tryGetFloatByKey(&mHipDropGravity, "HipDropGravity");
    mHipDropSpeedMax = 45.0;
    byaml.tryGetFloatByKey(&mHipDropSpeedMax, "HipDropSpeedMax");
    mHipDropLandCancelFrame = 24;
    byaml.tryGetIntByKey(&mHipDropLandCancelFrame, "HipDropLandCancelFrame");
    mHipDropHeight = 40.0;
    byaml.tryGetFloatByKey(&mHipDropHeight, "HipDropHeight");
    mHipDropMsginterval = 8;
    byaml.tryGetIntByKey(&mHipDropMsginterval, "HipDropMsginterval");
    mJumpHipDropPower = 40.0;
    byaml.tryGetFloatByKey(&mJumpHipDropPower, "JumpHipDropPower");
    mJumpHipDropPermitBeginFrame = 5;
    byaml.tryGetIntByKey(&mJumpHipDropPermitBeginFrame, "JumpHipDropPermitBeginFrame");
    mJumpHipDropPermitEndFrame = 30;
    byaml.tryGetIntByKey(&mJumpHipDropPermitEndFrame, "JumpHipDropPermitEndFrame");
    mHeadSlidingSpeed = 20.0;
    byaml.tryGetFloatByKey(&mHeadSlidingSpeed, "HeadSlidingSpeed");
    mHeadSlidingSpeedMin = 2.5;
    byaml.tryGetFloatByKey(&mHeadSlidingSpeedMin, "HeadSlidingSpeedMin");
    mHeadSlidingBrake = 0.5;
    byaml.tryGetFloatByKey(&mHeadSlidingBrake, "HeadSlidingBrake");
    mHeadSlidingSideAccel = 0.125;
    byaml.tryGetFloatByKey(&mHeadSlidingSideAccel, "HeadSlidingSideAccel");
    mHeadSlidingJump = 28.0;
    byaml.tryGetFloatByKey(&mHeadSlidingJump, "HeadSlidingJump");
    mHeadSlidingGravityAir = 2.0;
    byaml.tryGetFloatByKey(&mHeadSlidingGravityAir, "HeadSlidingGravityAir");
    mSwimCenterOffset = 80.0;
    byaml.tryGetFloatByKey(&mSwimCenterOffset, "SwimCenterOffset");
    mSwimWallCatchOffset = 100.0;
    byaml.tryGetFloatByKey(&mSwimWallCatchOffset, "SwimWallCatchOffset");
    mSwimRisePower = 0.6;
    byaml.tryGetFloatByKey(&mSwimRisePower, "SwimRisePower");
    mSwimRiseSpeedMax = 7.5;
    byaml.tryGetFloatByKey(&mSwimRiseSpeedMax, "SwimRiseSpeedMax");
    mSwimRiseFrame = 10;
    byaml.tryGetIntByKey(&mSwimRiseFrame, "SwimRiseFrame");
    mSwimGravity = 0.25;
    byaml.tryGetFloatByKey(&mSwimGravity, "SwimGravity");
    mSwimGravityWalk = 1.0;
    byaml.tryGetFloatByKey(&mSwimGravityWalk, "SwimGravityWalk");
    mSwimFallSpeedMax = 6.5;
    byaml.tryGetFloatByKey(&mSwimFallSpeedMax, "SwimFallSpeedMax");
    mSwimFloorAccelH = 0.125;
    byaml.tryGetFloatByKey(&mSwimFloorAccelH, "SwimFloorAccelH");
    mSwimFloorSpeedMaxH = 6.5;
    byaml.tryGetFloatByKey(&mSwimFloorSpeedMaxH, "SwimFloorSpeedMaxH");
    mSwimHighAccelH = 0.25;
    byaml.tryGetFloatByKey(&mSwimHighAccelH, "SwimHighAccelH");
    mSwimHighSpeedMaxH = 7.5;
    byaml.tryGetFloatByKey(&mSwimHighSpeedMaxH, "SwimHighSpeedMaxH");
    mSwimLowAccelH = 0.25;
    byaml.tryGetFloatByKey(&mSwimLowAccelH, "SwimLowAccelH");
    mSwimLowSpeedMaxH = 6.5;
    byaml.tryGetFloatByKey(&mSwimLowSpeedMaxH, "SwimLowSpeedMaxH");
    mSwimBrakeRateH = 0.975;
    byaml.tryGetFloatByKey(&mSwimBrakeRateH, "SwimBrakeRateH");
    mSwimWallHitSpeedMinH = 3.0;
    byaml.tryGetFloatByKey(&mSwimWallHitSpeedMinH, "SwimWallHitSpeedMinH");
    mSwimHighAccelPermitFrame = 35;
    byaml.tryGetIntByKey(&mSwimHighAccelPermitFrame, "SwimHighAccelPermitFrame");
    mSwimFlowFieldBlend = 0.776;
    byaml.tryGetFloatByKey(&mSwimFlowFieldBlend, "SwimFlowFieldBlend");
    mSwimWalkAnimMinRate = 0.2;
    byaml.tryGetFloatByKey(&mSwimWalkAnimMinRate, "SwimWalkAnimMinRate");
    mSwimWalkAnimMaxRate = 1.9;
    byaml.tryGetFloatByKey(&mSwimWalkAnimMaxRate, "SwimWalkAnimMaxRate");
    mSwimWalkMaxSpeed = 5.0;
    byaml.tryGetFloatByKey(&mSwimWalkMaxSpeed, "SwimWalkMaxSpeed");
    mSwimSpinCapUpPower = 10.0;
    byaml.tryGetFloatByKey(&mSwimSpinCapUpPower, "SwimSpinCapUpPower");
    mSwimSpinCapUpSpeedMax = 4.5;
    byaml.tryGetFloatByKey(&mSwimSpinCapUpSpeedMax, "SwimSpinCapUpSpeedMax");
    mSwimRotStartAngle = 1.0;
    byaml.tryGetFloatByKey(&mSwimRotStartAngle, "SwimRotStartAngle");
    mSwimRotFastAngle = 80.0;
    byaml.tryGetFloatByKey(&mSwimRotFastAngle, "SwimRotFastAngle");
    mSwimRotAccelFrame = 20;
    byaml.tryGetIntByKey(&mSwimRotAccelFrame, "SwimRotAccelFrame");
    mSwimRotAccelFrameFast = 1;
    byaml.tryGetIntByKey(&mSwimRotAccelFrameFast, "SwimRotAccelFrameFast");
    mSwimRotBrakeFrame = 30;
    byaml.tryGetIntByKey(&mSwimRotBrakeFrame, "SwimRotBrakeFrame");
    mSwimRotSpeedChangeStart = 3.0;
    byaml.tryGetFloatByKey(&mSwimRotSpeedChangeStart, "SwimRotSpeedChangeStart");
    mSwimRotSpeedForward = 3.0;
    byaml.tryGetFloatByKey(&mSwimRotSpeedForward, "SwimRotSpeedForward");
    mSwimRotSpeedMax = 7.5;
    byaml.tryGetFloatByKey(&mSwimRotSpeedMax, "SwimRotSpeedMax");
    mSwimSurfaceAccelH = 0.25;
    byaml.tryGetFloatByKey(&mSwimSurfaceAccelH, "SwimSurfaceAccelH");
    mSwimSurfaceSpeedMaxH = 9.0;
    byaml.tryGetFloatByKey(&mSwimSurfaceSpeedMaxH, "SwimSurfaceSpeedMaxH");
    mSwimSurfaceSpinCapFrame = 45;
    byaml.tryGetIntByKey(&mSwimSurfaceSpinCapFrame, "SwimSurfaceSpinCapFrame");
    mSwimSurfaceSpinCapSpeedMaxH = 13.0;
    byaml.tryGetFloatByKey(&mSwimSurfaceSpinCapSpeedMaxH, "SwimSurfaceSpinCapSpeedMaxH");
    mSwimSurfaceStartDist = 120.0;
    byaml.tryGetFloatByKey(&mSwimSurfaceStartDist, "SwimSurfaceStartDist");
    mSwimSurfaceEndDist = 200.0;
    byaml.tryGetFloatByKey(&mSwimSurfaceEndDist, "SwimSurfaceEndDist");
    mSwimSurfaceGravity = 0.125;
    byaml.tryGetFloatByKey(&mSwimSurfaceGravity, "SwimSurfaceGravity");
    mSwimSurfaceBaseHeight = 80.0;
    byaml.tryGetFloatByKey(&mSwimSurfaceBaseHeight, "SwimSurfaceBaseHeight");
    mSwimSurfaceSpring = 0.05;
    byaml.tryGetFloatByKey(&mSwimSurfaceSpring, "SwimSurfaceSpring");
    mSwimSurfaceDamper = 0.949;
    byaml.tryGetFloatByKey(&mSwimSurfaceDamper, "SwimSurfaceDamper");
    mSwimSurfaceDamperStart = 25;
    byaml.tryGetIntByKey(&mSwimSurfaceDamperStart, "SwimSurfaceDamperStart");
    mSwimSurfaceDamperFrame = 25;
    byaml.tryGetIntByKey(&mSwimSurfaceDamperFrame, "SwimSurfaceDamperFrame");
    mSwimSurfaceEnableJumpHeight = 160.0;
    byaml.tryGetFloatByKey(&mSwimSurfaceEnableJumpHeight, "SwimSurfaceEnableJumpHeight");
    mSwimSurfacePreInputJumpFrame = 8;
    byaml.tryGetIntByKey(&mSwimSurfacePreInputJumpFrame, "SwimSurfacePreInputJumpFrame");
    mSwimSurfaceMoveSpring = 0.01;
    byaml.tryGetFloatByKey(&mSwimSurfaceMoveSpring, "SwimSurfaceMoveSpring");
    mSwimSurfaceMoveDamper = 0.94;
    byaml.tryGetFloatByKey(&mSwimSurfaceMoveDamper, "SwimSurfaceMoveDamper");
    mSwimSurfaceMoveBaseHeight = 80.0;
    byaml.tryGetFloatByKey(&mSwimSurfaceMoveBaseHeight, "SwimSurfaceMoveBaseHeight");
    mSwimRunSurfaceBaseHeight = 0.0;
    byaml.tryGetFloatByKey(&mSwimRunSurfaceBaseHeight, "SwimRunSurfaceBaseHeight");
    mSwimRunSurfaceApproachRate = 0.5;
    byaml.tryGetFloatByKey(&mSwimRunSurfaceApproachRate, "SwimRunSurfaceApproachRate");
    mSwimRunSurfaceApproachLimit = 5.0;
    byaml.tryGetFloatByKey(&mSwimRunSurfaceApproachLimit, "SwimRunSurfaceApproachLimit");
    mSwimRunSurfaceBrakeBorder = 35.0;
    byaml.tryGetFloatByKey(&mSwimRunSurfaceBrakeBorder, "SwimRunSurfaceBrakeBorder");
    mSwimRunSurfaceBrakeH = 0.995;
    byaml.tryGetFloatByKey(&mSwimRunSurfaceBrakeH, "SwimRunSurfaceBrakeH");
    mSwimRunSurfaceApproachBorderMax = 38.0;
    byaml.tryGetFloatByKey(&mSwimRunSurfaceApproachBorderMax, "SwimRunSurfaceApproachBorderMax");
    mSwimRunSurfaceApproachBorderMin = 30.0;
    byaml.tryGetFloatByKey(&mSwimRunSurfaceApproachBorderMin, "SwimRunSurfaceApproachBorderMin");
    mSwimRunSurfaceApproachRateMin = 0.06;
    byaml.tryGetFloatByKey(&mSwimRunSurfaceApproachRateMin, "SwimRunSurfaceApproachRateMin");
    mSwimFallInSpeed = 15.0;
    byaml.tryGetFloatByKey(&mSwimFallInSpeed, "SwimFallInSpeed");
    mSwimFallInBrakeH = 0.95;
    byaml.tryGetFloatByKey(&mSwimFallInBrakeH, "SwimFallInBrakeH");
    mSwimFallInBrakeV = 0.91;
    byaml.tryGetFloatByKey(&mSwimFallInBrakeV, "SwimFallInBrakeV");
    mSwimHeadInBrakeH = 0.98;
    byaml.tryGetFloatByKey(&mSwimHeadInBrakeH, "SwimHeadInBrakeH");
    mSwimHeadInBrakeV = 0.92;
    byaml.tryGetFloatByKey(&mSwimHeadInBrakeV, "SwimHeadInBrakeV");
    mSwimHeadInRisePower = 0.6;
    byaml.tryGetFloatByKey(&mSwimHeadInRisePower, "SwimHeadInRisePower");
    mSwimHeadInRiseSpeedMax = 10.0;
    byaml.tryGetFloatByKey(&mSwimHeadInRiseSpeedMax, "SwimHeadInRiseSpeedMax");
    mSwimHeadInSurfaceHeight = 50.0;
    byaml.tryGetFloatByKey(&mSwimHeadInSurfaceHeight, "SwimHeadInSurfaceHeight");
    mSwimFallInForceSurfaceFrame = 10;
    byaml.tryGetIntByKey(&mSwimFallInForceSurfaceFrame, "SwimFallInForceSurfaceFrame");
    mSwimFallInvalidJumpFrame = 8;
    byaml.tryGetIntByKey(&mSwimFallInvalidJumpFrame, "SwimFallInvalidJumpFrame");
    mSwimDiveStartSpeed = 26.5;
    byaml.tryGetFloatByKey(&mSwimDiveStartSpeed, "SwimDiveStartSpeed");
    mSwimDiveBrake = 0.875;
    byaml.tryGetFloatByKey(&mSwimDiveBrake, "SwimDiveBrake");
    mSwimDiveEndSpeed = 1.25;
    byaml.tryGetFloatByKey(&mSwimDiveEndSpeed, "SwimDiveEndSpeed");
    mSwimDiveLandCount = 0;
    byaml.tryGetIntByKey(&mSwimDiveLandCount, "SwimDiveLandCount");
    mSwimDiveLandCancelFrame = 15;
    byaml.tryGetIntByKey(&mSwimDiveLandCancelFrame, "SwimDiveLandCancelFrame");
    mSwimDiveNoBrakeFrame = 15;
    byaml.tryGetIntByKey(&mSwimDiveNoBrakeFrame, "SwimDiveNoBrakeFrame");
    mSwimDiveButtonValidFrame = 10;
    byaml.tryGetIntByKey(&mSwimDiveButtonValidFrame, "SwimDiveButtonValidFrame");
    mSwimDiveEndFrame = 5;
    byaml.tryGetIntByKey(&mSwimDiveEndFrame, "SwimDiveEndFrame");
    mSwimDiveInBrakeH = 0.97;
    byaml.tryGetFloatByKey(&mSwimDiveInBrakeH, "SwimDiveInBrakeH");
    mSwimDiveInBrakeV = 0.987;
    byaml.tryGetFloatByKey(&mSwimDiveInBrakeV, "SwimDiveInBrakeV");
    mSwimDiveInRisePower = 0.5;
    byaml.tryGetFloatByKey(&mSwimDiveInRisePower, "SwimDiveInRisePower");
    mSwimDiveInRiseSpeedMax = 3.0;
    byaml.tryGetFloatByKey(&mSwimDiveInRiseSpeedMax, "SwimDiveInRiseSpeedMax");
    mSwimDiveInSurfaceHeight = 50.0;
    byaml.tryGetFloatByKey(&mSwimDiveInSurfaceHeight, "SwimDiveInSurfaceHeight");
    mSwimDiveInKeepFrame = 10;
    byaml.tryGetIntByKey(&mSwimDiveInKeepFrame, "SwimDiveInKeepFrame");
    mSwimHeadSlidingFrame = 15;
    byaml.tryGetIntByKey(&mSwimHeadSlidingFrame, "SwimHeadSlidingFrame");
    mSwimHeadSlidingBrakeFrame = 45;
    byaml.tryGetIntByKey(&mSwimHeadSlidingBrakeFrame, "SwimHeadSlidingBrakeFrame");
    mSwimHeadSlidingSpeed = 15.0;
    byaml.tryGetFloatByKey(&mSwimHeadSlidingSpeed, "SwimHeadSlidingSpeed");
    mSwimHeadSlidingSpeedEnd = 10.0;
    byaml.tryGetFloatByKey(&mSwimHeadSlidingSpeedEnd, "SwimHeadSlidingSpeedEnd");
    mSwimHeadSlidingBrake = 0.0;
    byaml.tryGetFloatByKey(&mSwimHeadSlidingBrake, "SwimHeadSlidingBrake");
    mSwimHeadSlidingSideAccel = 0.125;
    byaml.tryGetFloatByKey(&mSwimHeadSlidingSideAccel, "SwimHeadSlidingSideAccel");
    mSwimHeadSlidingJump = 0.0;
    byaml.tryGetFloatByKey(&mSwimHeadSlidingJump, "SwimHeadSlidingJump");
    mSwimHeadSlidingGravity = 0.0;
    byaml.tryGetFloatByKey(&mSwimHeadSlidingGravity, "SwimHeadSlidingGravity");
    mSwimHeadSlidingEndBrakeFrame = 30;
    byaml.tryGetIntByKey(&mSwimHeadSlidingEndBrakeFrame, "SwimHeadSlidingEndBrakeFrame");
    mSwimHeadSlidingEndSpeedMin = 5;
    byaml.tryGetIntByKey(&mSwimHeadSlidingEndSpeedMin, "SwimHeadSlidingEndSpeedMin");
    mSwimJumpHipDropSpeed = 27.0;
    byaml.tryGetFloatByKey(&mSwimJumpHipDropSpeed, "SwimJumpHipDropSpeed");
    mSwimJumpHipDropBrakeV = 0.98;
    byaml.tryGetFloatByKey(&mSwimJumpHipDropBrakeV, "SwimJumpHipDropBrakeV");
    mSwimJumpHipDropBrakeVCeiling = 0.5;
    byaml.tryGetFloatByKey(&mSwimJumpHipDropBrakeVCeiling, "SwimJumpHipDropBrakeVCeiling");
    mSwimJumpHipDropGravity = 0.2;
    byaml.tryGetFloatByKey(&mSwimJumpHipDropGravity, "SwimJumpHipDropGravity");
    mSwimJumpHipDropCancelSpeed = 7.5;
    byaml.tryGetFloatByKey(&mSwimJumpHipDropCancelSpeed, "SwimJumpHipDropCancelSpeed");
    mSwimJumpHipDropAccelH = 0.25;
    byaml.tryGetFloatByKey(&mSwimJumpHipDropAccelH, "SwimJumpHipDropAccelH");
    mSwimJumpHipDropMoveSpeedH = 3.0;
    byaml.tryGetFloatByKey(&mSwimJumpHipDropMoveSpeedH, "SwimJumpHipDropMoveSpeedH");
    mSwimJumpHipDropPopSpeed = 8.0;
    byaml.tryGetFloatByKey(&mSwimJumpHipDropPopSpeed, "SwimJumpHipDropPopSpeed");
    mSwimJumpHipDropPopJumpAdd = 7.0;
    byaml.tryGetFloatByKey(&mSwimJumpHipDropPopJumpAdd, "SwimJumpHipDropPopJumpAdd");
    mSwimTramplePower = 8.0;
    byaml.tryGetFloatByKey(&mSwimTramplePower, "SwimTramplePower");
    mDiveTramplePower = 11.0;
    byaml.tryGetFloatByKey(&mDiveTramplePower, "DiveTramplePower");
    mDiveTrampleCancelFrame = 20;
    byaml.tryGetIntByKey(&mDiveTrampleCancelFrame, "DiveTrampleCancelFrame");
    mDamageSwimPushPower = 3.0;
    byaml.tryGetFloatByKey(&mDamageSwimPushPower, "DamageSwimPushPower");
    mDamageSwimGravity = 0.02;
    byaml.tryGetFloatByKey(&mDamageSwimGravity, "DamageSwimGravity");
    mDamageSwimCancelFrame = 50;
    byaml.tryGetIntByKey(&mDamageSwimCancelFrame, "DamageSwimCancelFrame");
    mDamageSwimSurfaceGravity = 0.95;
    byaml.tryGetFloatByKey(&mDamageSwimSurfaceGravity, "DamageSwimSurfaceGravity");
    mDamageSwimSurfaceHopPower = 20.0;
    byaml.tryGetFloatByKey(&mDamageSwimSurfaceHopPower, "DamageSwimSurfaceHopPower");
    mDamageSwimSurfacePushPower = 4.0;
    byaml.tryGetFloatByKey(&mDamageSwimSurfacePushPower, "DamageSwimSurfacePushPower");
    mDamageSwimSurfaceLandSpeed = 5.0;
    byaml.tryGetFloatByKey(&mDamageSwimSurfaceLandSpeed, "DamageSwimSurfaceLandSpeed");
    mDamageSwimSurfaceLandBrake = 0.05;
    byaml.tryGetFloatByKey(&mDamageSwimSurfaceLandBrake, "DamageSwimSurfaceLandBrake");
    mDamageSwimSurfaceLandEndSpeed = 1.0;
    byaml.tryGetFloatByKey(&mDamageSwimSurfaceLandEndSpeed, "DamageSwimSurfaceLandEndSpeed");
    mDamageSwimSurfaceCancelFrame = 40;
    byaml.tryGetIntByKey(&mDamageSwimSurfaceCancelFrame, "DamageSwimSurfaceCancelFrame");
    mDamageSwimBrakeRateGround = 0.95;
    byaml.tryGetFloatByKey(&mDamageSwimBrakeRateGround, "DamageSwimBrakeRateGround");
    mOxygenReduceFrame = 1380;
    byaml.tryGetIntByKey(&mOxygenReduceFrame, "OxygenReduceFrame");
    mOxygenNoReduceFrame = 60;
    byaml.tryGetIntByKey(&mOxygenNoReduceFrame, "OxygenNoReduceFrame");
    mOxygenRecoveryFrame = 30;
    byaml.tryGetIntByKey(&mOxygenRecoveryFrame, "OxygenRecoveryFrame");
    mOxygenDamageinterval = 300;
    byaml.tryGetIntByKey(&mOxygenDamageinterval, "OxygenDamageinterval");
    mIceWaterDamageinterval = 300;
    byaml.tryGetIntByKey(&mIceWaterDamageinterval, "IceWaterDamageinterval");
    mIceWaterRecoveryFrame = 70;
    byaml.tryGetIntByKey(&mIceWaterRecoveryFrame, "IceWaterRecoveryFrame");
    mMoveAnimSpeedMax = 25.0;
    byaml.tryGetFloatByKey(&mMoveAnimSpeedMax, "MoveAnimSpeedMax");
    mAnimFrameRateSpeedMin = 6.0;
    byaml.tryGetFloatByKey(&mAnimFrameRateSpeedMin, "AnimFrameRateSpeedMin");
    mRunBorderSpeed = 8.0;
    byaml.tryGetFloatByKey(&mRunBorderSpeed, "RunBorderSpeed");
    mRunBlendRange = 2.0;
    byaml.tryGetFloatByKey(&mRunBlendRange, "RunBlendRange");
    mDashBorderSpeed = 15.0;
    byaml.tryGetFloatByKey(&mDashBorderSpeed, "DashBorderSpeed");
    mDashBlendRange = 1.0;
    byaml.tryGetFloatByKey(&mDashBlendRange, "DashBlendRange");
    mDashFastBorderSpeed = 20.0;
    byaml.tryGetFloatByKey(&mDashFastBorderSpeed, "DashFastBorderSpeed");
    mDashFastBlendRange = 1.0;
    byaml.tryGetFloatByKey(&mDashFastBlendRange, "DashFastBlendRange");
    mAnimFrameRateSpeedMax = 26.0;
    byaml.tryGetFloatByKey(&mAnimFrameRateSpeedMax, "AnimFrameRateSpeedMax");
    mAnimFrameRateRunStart = 2.0;
    byaml.tryGetFloatByKey(&mAnimFrameRateRunStart, "AnimFrameRateRunStart");
    mAnimFrameRateMinRun = 1.0;
    byaml.tryGetFloatByKey(&mAnimFrameRateMinRun, "AnimFrameRateMinRun");
    mAnimFrameRateMaxRun = 3.5;
    byaml.tryGetFloatByKey(&mAnimFrameRateMaxRun, "AnimFrameRateMaxRun");
    mAnimFrameRateMaxDash = 4.0;
    byaml.tryGetFloatByKey(&mAnimFrameRateMaxDash, "AnimFrameRateMaxDash");
    mAnimFrameRateMaxDashFast = 5.0;
    byaml.tryGetFloatByKey(&mAnimFrameRateMaxDashFast, "AnimFrameRateMaxDashFast");
    mRunStartPlayFrameScale = 1.0;
    byaml.tryGetFloatByKey(&mRunStartPlayFrameScale, "RunStartPlayFrameScale");
    mRunStartBlendFrame = 5;
    byaml.tryGetIntByKey(&mRunStartBlendFrame, "RunStartBlendFrame");
    mDamageFireRunAnimRate = 0.3;
    byaml.tryGetFloatByKey(&mDamageFireRunAnimRate, "DamageFireRunAnimRate");
    mRunSkateAnimSpeedOffset = 5.0;
    byaml.tryGetFloatByKey(&mRunSkateAnimSpeedOffset, "RunSkateAnimSpeedOffset");
    mAnimFrameRateRange2D = 10.0;
    byaml.tryGetFloatByKey(&mAnimFrameRateRange2D, "AnimFrameRateRange2D");
    mAnimFrameRateMinRun2D = 1.0;
    byaml.tryGetFloatByKey(&mAnimFrameRateMinRun2D, "AnimFrameRateMinRun2D");
    mAnimFrameRateMaxRun2D = 2.5;
    byaml.tryGetFloatByKey(&mAnimFrameRateMaxRun2D, "AnimFrameRateMaxRun2D");
    mIKBlendFrameRun = 60;
    byaml.tryGetIntByKey(&mIKBlendFrameRun, "IKBlendFrameRun");
    mIKBlendRateRunMin = 0.85;
    byaml.tryGetFloatByKey(&mIKBlendRateRunMin, "IKBlendRateRunMin");
    mIKBlendRateRunMax = 0.95;
    byaml.tryGetFloatByKey(&mIKBlendRateRunMax, "IKBlendRateRunMax");
    mRollingAnimBorderSpeedMin = 5.0;
    byaml.tryGetFloatByKey(&mRollingAnimBorderSpeedMin, "RollingAnimBorderSpeedMin");
    mRollingAnimBorderSpeedMax = 35.0;
    byaml.tryGetFloatByKey(&mRollingAnimBorderSpeedMax, "RollingAnimBorderSpeedMax");
    mRollingAnimFrameRateMin = 0.2;
    byaml.tryGetFloatByKey(&mRollingAnimFrameRateMin, "RollingAnimFrameRateMin");
    mRollingAnimFrameRateMax = 1.0;
    byaml.tryGetFloatByKey(&mRollingAnimFrameRateMax, "RollingAnimFrameRateMax");
    mSwimPaddleAniminterval = 32;
    byaml.tryGetIntByKey(&mSwimPaddleAniminterval, "SwimPaddleAniminterval");
    mSwimPaddleAnimRateintervalMax = 22;
    byaml.tryGetIntByKey(&mSwimPaddleAnimRateintervalMax, "SwimPaddleAnimRateintervalMax");
    mSwimPaddleAnimRateintervalMin = 5;
    byaml.tryGetIntByKey(&mSwimPaddleAnimRateintervalMin, "SwimPaddleAnimRateintervalMin");
    mSwimPaddleAnimMaxRate = 3.0;
    byaml.tryGetFloatByKey(&mSwimPaddleAnimMaxRate, "SwimPaddleAnimMaxRate");
    mSwimBentForwardMax = 30.0;
    byaml.tryGetFloatByKey(&mSwimBentForwardMax, "SwimBentForwardMax");
    mSwimBentForwardBlendRate = 0.05;
    byaml.tryGetFloatByKey(&mSwimBentForwardBlendRate, "SwimBentForwardBlendRate");
    mSwimBentSideMax = 60.0;
    byaml.tryGetFloatByKey(&mSwimBentSideMax, "SwimBentSideMax");
    mSwimBentSpineMax = 40.0;
    byaml.tryGetFloatByKey(&mSwimBentSpineMax, "SwimBentSpineMax");
    mSwimBentSideBlendRate = 0.05;
    byaml.tryGetFloatByKey(&mSwimBentSideBlendRate, "SwimBentSideBlendRate");
    mSwimBentFrontMax = 45.0;
    byaml.tryGetFloatByKey(&mSwimBentFrontMax, "SwimBentFrontMax");
    mSwimBentFrontBlendRate = 0.04;
    byaml.tryGetFloatByKey(&mSwimBentFrontBlendRate, "SwimBentFrontBlendRate");
    mSwimWalkAnimSpeedMax = 6.5;
    byaml.tryGetFloatByKey(&mSwimWalkAnimSpeedMax, "SwimWalkAnimSpeedMax");
    mSwimWalkAnimSpeedMin = 1.0;
    byaml.tryGetFloatByKey(&mSwimWalkAnimSpeedMin, "SwimWalkAnimSpeedMin");
    mSwimWalkAnimFrameRateMax = 1.75;
    byaml.tryGetFloatByKey(&mSwimWalkAnimFrameRateMax, "SwimWalkAnimFrameRateMax");
    mSwimWalkAnimFrameRateMin = 0.5;
    byaml.tryGetFloatByKey(&mSwimWalkAnimFrameRateMin, "SwimWalkAnimFrameRateMin");
    mSandSinkBorderMin = 0.0;
    byaml.tryGetFloatByKey(&mSandSinkBorderMin, "SandSinkBorderMin");
    mSandSinkBorderMax = 0.5;
    byaml.tryGetFloatByKey(&mSandSinkBorderMax, "SandSinkBorderMax");
    mSandSinkBorderRateMin = 0.0;
    byaml.tryGetFloatByKey(&mSandSinkBorderRateMin, "SandSinkBorderRateMin");
    mSandSinkBorderRateMax = 0.5;
    byaml.tryGetFloatByKey(&mSandSinkBorderRateMax, "SandSinkBorderRateMax");
    mSandSinkFrameRateMin = 4.0;
    byaml.tryGetFloatByKey(&mSandSinkFrameRateMin, "SandSinkFrameRateMin");
    mSandSinkFrameRateMax = 0.5;
    byaml.tryGetFloatByKey(&mSandSinkFrameRateMax, "SandSinkFrameRateMax");
    mLookAtEyeKeepFrame = 30;
    byaml.tryGetIntByKey(&mLookAtEyeKeepFrame, "LookAtEyeKeepFrame");
    mLookAtEyeKeepFrameInSight = 0;
    byaml.tryGetIntByKey(&mLookAtEyeKeepFrameInSight, "LookAtEyeKeepFrameInSight");
    mLookAtEyeKeepFrameWait = 120;
    byaml.tryGetIntByKey(&mLookAtEyeKeepFrameWait, "LookAtEyeKeepFrameWait");
    mLookAtEyeDistance = 500.0;
    byaml.tryGetFloatByKey(&mLookAtEyeDistance, "LookAtEyeDistance");
    mLookAtEyeAngleMinH = 35.0;
    byaml.tryGetFloatByKey(&mLookAtEyeAngleMinH, "LookAtEyeAngleMinH");
    mLookAtEyeAngleMinInSightH = 10.0;
    byaml.tryGetFloatByKey(&mLookAtEyeAngleMinInSightH, "LookAtEyeAngleMinInSightH");
    mLookAtEyeAngleMaxH = 85.0;
    byaml.tryGetFloatByKey(&mLookAtEyeAngleMaxH, "LookAtEyeAngleMaxH");
    mLookAtEyeAngleMinV = 10.0;
    byaml.tryGetFloatByKey(&mLookAtEyeAngleMinV, "LookAtEyeAngleMinV");
    mLookAtEyeAngleMinInSightV = 10.0;
    byaml.tryGetFloatByKey(&mLookAtEyeAngleMinInSightV, "LookAtEyeAngleMinInSightV");
    mLookAtEyeAngleMaxV = 60.0;
    byaml.tryGetFloatByKey(&mLookAtEyeAngleMaxV, "LookAtEyeAngleMaxV");
    mTiltEyeBorderStart = 0.9;
    byaml.tryGetFloatByKey(&mTiltEyeBorderStart, "TiltEyeBorderStart");
    mTiltEyeBorderEnd = 0.25;
    byaml.tryGetFloatByKey(&mTiltEyeBorderEnd, "TiltEyeBorderEnd");
    mTiltEyeAngleScale = 0.4;
    byaml.tryGetFloatByKey(&mTiltEyeAngleScale, "TiltEyeAngleScale");
    mCenterTiltRateMax = 0.0;
    byaml.tryGetFloatByKey(&mCenterTiltRateMax, "CenterTiltRateMax");
    mNoseChildLocalOffset = 50.0;
    byaml.tryGetFloatByKey(&mNoseChildLocalOffset, "NoseChildLocalOffset");
    mNoseStability = 0.1;
    byaml.tryGetFloatByKey(&mNoseStability, "NoseStability");
    mNoseFriction = 0.8;
    byaml.tryGetFloatByKey(&mNoseFriction, "NoseFriction");
    mNoseLimitDegree = 45.0;
    byaml.tryGetFloatByKey(&mNoseLimitDegree, "NoseLimitDegree");
    mMustacheChildLocalOffset = 100.0;
    byaml.tryGetFloatByKey(&mMustacheChildLocalOffset, "MustacheChildLocalOffset");
    mMustacheStability = 0.3;
    byaml.tryGetFloatByKey(&mMustacheStability, "MustacheStability");
    mMustacheFriction = 0.8;
    byaml.tryGetFloatByKey(&mMustacheFriction, "MustacheFriction");
    mMustacheLimitDegree = 10.0;
    byaml.tryGetFloatByKey(&mMustacheLimitDegree, "MustacheLimitDegree");
    mCapinterpolateFrame = 15;
    byaml.tryGetIntByKey(&mCapinterpolateFrame, "CapinterpolateFrame");
    mCapChildLocalOffset = 100.0;
    byaml.tryGetFloatByKey(&mCapChildLocalOffset, "CapChildLocalOffset");
    mCapStability = 0.75;
    byaml.tryGetFloatByKey(&mCapStability, "CapStability");
    mCapFriction = 0.2;
    byaml.tryGetFloatByKey(&mCapFriction, "CapFriction");
    mCapLimitDegree = 15.0;
    byaml.tryGetFloatByKey(&mCapLimitDegree, "CapLimitDegree");
    mCapTransStability = 0.95;
    byaml.tryGetFloatByKey(&mCapTransStability, "CapTransStability");
    mCapTransFriction = 0.25;
    byaml.tryGetFloatByKey(&mCapTransFriction, "CapTransFriction");
    mCapTransLimit = 10.0;
    byaml.tryGetFloatByKey(&mCapTransLimit, "CapTransLimit");
    mCapManHeroEyesWaitAppearFrame = 330;
    byaml.tryGetIntByKey(&mCapManHeroEyesWaitAppearFrame, "CapManHeroEyesWaitAppearFrame");
    mDeadWipeStartDamage = 30;
    byaml.tryGetIntByKey(&mDeadWipeStartDamage, "DeadWipeStartDamage");
    mDeadWipeWaitDamage = 96;
    byaml.tryGetIntByKey(&mDeadWipeWaitDamage, "DeadWipeWaitDamage");
    mDeadWipeStartAbyss = 30;
    byaml.tryGetIntByKey(&mDeadWipeStartAbyss, "DeadWipeStartAbyss");
    mDeadWipeWaitAbyss = 30;
    byaml.tryGetIntByKey(&mDeadWipeWaitAbyss, "DeadWipeWaitAbyss");
    mDeadWipeStartAbyssWithCapMsg = 90;
    byaml.tryGetIntByKey(&mDeadWipeStartAbyssWithCapMsg, "DeadWipeStartAbyssWithCapMsg");
    mDeadWipeWaitAbyssWithCapMsg = 30;
    byaml.tryGetIntByKey(&mDeadWipeWaitAbyssWithCapMsg, "DeadWipeWaitAbyssWithCapMsg");
    mDeadWipeStartPress = 40;
    byaml.tryGetIntByKey(&mDeadWipeStartPress, "DeadWipeStartPress");
    mDeadWipeWaitPress = 96;
    byaml.tryGetIntByKey(&mDeadWipeWaitPress, "DeadWipeWaitPress");
    mDeadWipeStartSandSink = 30;
    byaml.tryGetIntByKey(&mDeadWipeStartSandSink, "DeadWipeStartSandSink");
    mDeadWipeWaitSandSink = 96;
    byaml.tryGetIntByKey(&mDeadWipeWaitSandSink, "DeadWipeWaitSandSink");
    mDeadWipeStartNoOxygen = 30;
    byaml.tryGetIntByKey(&mDeadWipeStartNoOxygen, "DeadWipeStartNoOxygen");
    mDeadWipeWaitNoOxygen = 96;
    byaml.tryGetIntByKey(&mDeadWipeWaitNoOxygen, "DeadWipeWaitNoOxygen");
    mDeadWipeStartIceWater = 30;
    byaml.tryGetIntByKey(&mDeadWipeStartIceWater, "DeadWipeStartIceWater");
    mDeadWipeWaitIceWater = 96;
    byaml.tryGetIntByKey(&mDeadWipeWaitIceWater, "DeadWipeWaitIceWater");
    mCoinDashSpeed = 6.0;
    byaml.tryGetFloatByKey(&mCoinDashSpeed, "CoinDashSpeed");
    mCoinDashSpeedLimit = 6.0;
    byaml.tryGetFloatByKey(&mCoinDashSpeedLimit, "CoinDashSpeedLimit");
    mAdditionalSpeedLimit = 30.0;
    byaml.tryGetFloatByKey(&mAdditionalSpeedLimit, "AdditionalSpeedLimit");
}

float PlayerConst::getGravity() const {
    return mGravity;
}
float PlayerConst::getFrictionAttack() const {
    return mFrictionAttack;
}
float PlayerConst::getPushPower() const {
    return mPushPower;
}
float PlayerConst::getWaitPoseDegreeMax() const {
    return mWaitPoseDegreeMax;
}
float PlayerConst::getHillPoseDegreeMax() const {
    return mHillPoseDegreeMax;
}
float PlayerConst::getTiltPoseDegreeMax() const {
    return mTiltPoseDegreeMax;
}
float PlayerConst::getSlerpQuatRate() const {
    return mSlerpQuatRate;
}
float PlayerConst::getSlerpQuatRateWait() const {
    return mSlerpQuatRateWait;
}
float PlayerConst::getSlerpQuatGrav() const {
    return mSlerpQuatGrav;
}
int PlayerConst::getPreInputFrameCapThrow() const {
    return mPreInputFrameCapThrow;
}
int PlayerConst::getEnableActionFrameCapCatch() const {
    return mEnableActionFrameCapCatch;
}
float PlayerConst::getJumpPowerCapCatch() const {
    return mJumpPowerCapCatch;
}
float PlayerConst::getJumpGravityCapCatch() const {
    return mJumpGravityCapCatch;
}
int PlayerConst::getRunTimeContinuousThrow() const {
    return mRunTimeContinuousThrow;
}
float PlayerConst::getRunSpeedMaxContinuousThrow() const {
    return mRunSpeedMaxContinuousThrow;
}
int PlayerConst::getRunAccelFrameContinuousThrow() const {
    return mRunAccelFrameContinuousThrow;
}
float PlayerConst::getSeparateCheckHeight() const {
    return mSeparateCheckHeight;
}
float PlayerConst::getSeparateOffsetLerpRate() const {
    return mSeparateOffsetLerpRate;
}
float PlayerConst::getSeparateEnableThrowHeight() const {
    return mSeparateEnableThrowHeight;
}
float PlayerConst::getTall() const {
    return mTall;
}
float PlayerConst::getCollisionRadius() const {
    return mCollisionRadius;
}
float PlayerConst::getCollisionRadiusSquat() const {
    return mCollisionRadiusSquat;
}
float PlayerConst::getCollisionRadiusStand() const {
    return mCollisionRadiusStand;
}
float PlayerConst::getCollisionSmallStepHeight() const {
    return mCollisionSmallStepHeight;
}
float PlayerConst::getCollisionResetLimit() const {
    return mCollisionResetLimit;
}
float PlayerConst::getReflectCeilingPower() const {
    return mReflectCeilingPower;
}
float PlayerConst::getReflectTossPower() const {
    return mReflectTossPower;
}
float PlayerConst::getReflectUpperPunchScaleH() const {
    return mReflectUpperPunchScaleH;
}
float PlayerConst::getCollisionHitDownAngleH() const {
    return mCollisionHitDownAngleH;
}
float PlayerConst::getCollisionHitDownEscapeAngleV() const {
    return mCollisionHitDownEscapeAngleV;
}
float PlayerConst::getShadowDropHeightScale() const {
    return mShadowDropHeightScale;
}
float PlayerConst::getShadowDropNormalAdd() const {
    return mShadowDropNormalAdd;
}
float PlayerConst::getShadowDropLengthMin() const {
    return mShadowDropLengthMin;
}
float PlayerConst::getShadowDropLengthMax() const {
    return mShadowDropLengthMax;
}
float PlayerConst::getShadowDropLengthExtend() const {
    return mShadowDropLengthExtend;
}
float PlayerConst::getGravityDamage() const {
    return mGravityDamage;
}
float PlayerConst::getHopPowerDamage() const {
    return mHopPowerDamage;
}
float PlayerConst::getPushPowerDamage() const {
    return mPushPowerDamage;
}
int PlayerConst::getDamageCancelFrame() const {
    return mDamageCancelFrame;
}
int PlayerConst::getDamageInvalidCount() const {
    return mDamageInvalidCount;
}
int PlayerConst::getDamageInvalidCountRecovery() const {
    return mDamageInvalidCountRecovery;
}
int PlayerConst::getDamageInvalidCountAbyss() const {
    return mDamageInvalidCountAbyss;
}
float PlayerConst::getNormalMinSpeed2D() const {
    return mNormalMinSpeed2D;
}
float PlayerConst::getNormalMaxSpeed2D() const {
    return mNormalMaxSpeed2D;
}
float PlayerConst::getDashMaxSpeed2D() const {
    return mDashMaxSpeed2D;
}
int PlayerConst::getNormalAccelFrame2D() const {
    return mNormalAccelFrame2D;
}
int PlayerConst::getDashAccelFrame2D() const {
    return mDashAccelFrame2D;
}
int PlayerConst::getNormalDashAnimFrame2D() const {
    return mNormalDashAnimFrame2D;
}
int PlayerConst::getNormalBrakeFrame2D() const {
    return mNormalBrakeFrame2D;
}
int PlayerConst::getStickOnBrakeFrame2D() const {
    return mStickOnBrakeFrame2D;
}
int PlayerConst::getBrakeTurnStartFrame2D() const {
    return mBrakeTurnStartFrame2D;
}
float PlayerConst::getTurnEndSpeedRate2D() const {
    return mTurnEndSpeedRate2D;
}
float PlayerConst::getJumpPowerMin2DArea() const {
    return mJumpPowerMin2DArea;
}
float PlayerConst::getJumpPowerMax2DArea() const {
    return mJumpPowerMax2DArea;
}
float PlayerConst::getJumpPowerMinBorder2D() const {
    return mJumpPowerMinBorder2D;
}
float PlayerConst::getJumpPowerMaxBorder2D() const {
    return mJumpPowerMaxBorder2D;
}
float PlayerConst::getGravityMove() const {
    return mGravityMove;
}
float PlayerConst::getNormalMaxSpeed() const {
    return mNormalMaxSpeed;
}
float PlayerConst::getNormalMinSpeed() const {
    return mNormalMinSpeed;
}
int PlayerConst::getNormalAccelFrame() const {
    return mNormalAccelFrame;
}
float PlayerConst::getRunAccelAverageScale() const {
    return mRunAccelAverageScale;
}
int PlayerConst::getNormalBrakeFrame() const {
    return mNormalBrakeFrame;
}
float PlayerConst::getDashJudgeSpeed() const {
    return mDashJudgeSpeed;
}
int PlayerConst::getStickOnBrakeFrame() const {
    return mStickOnBrakeFrame;
}
int PlayerConst::getNormalDashAnimFrame() const {
    return mNormalDashAnimFrame;
}
float PlayerConst::getRunAfterTurnSpeedMax() const {
    return mRunAfterTurnSpeedMax;
}
float PlayerConst::getRunAfterTurnScale() const {
    return mRunAfterTurnScale;
}
int PlayerConst::getRunAfterTurnFrame() const {
    return mRunAfterTurnFrame;
}
int PlayerConst::getBrakeTurnStartFrame() const {
    return mBrakeTurnStartFrame;
}
float PlayerConst::getBrakeOnSpeedRate() const {
    return mBrakeOnSpeedRate;
}
int PlayerConst::getBrakeOnCounterBorder() const {
    return mBrakeOnCounterBorder;
}
int PlayerConst::getWallPushFrame() const {
    return mWallPushFrame;
}
int PlayerConst::getRunDeepDownFrame() const {
    return mRunDeepDownFrame;
}
int PlayerConst::getRunDeepDownMargine() const {
    return mRunDeepDownMargine;
}
int PlayerConst::getQuickTurnJumpFrame() const {
    return mQuickTurnJumpFrame;
}
int PlayerConst::getRoundAccelFrame() const {
    return mRoundAccelFrame;
}
int PlayerConst::getRoundBrakeFrame() const {
    return mRoundBrakeFrame;
}
float PlayerConst::getRoundFastDegree() const {
    return mRoundFastDegree;
}
int PlayerConst::getRoundAccelFrameFast() const {
    return mRoundAccelFrameFast;
}
float PlayerConst::getRoundMinDegree() const {
    return mRoundMinDegree;
}
int PlayerConst::getRoundBrakeFrameForce() const {
    return mRoundBrakeFrameForce;
}
float PlayerConst::getRoundFastDegreeForce() const {
    return mRoundFastDegreeForce;
}
float PlayerConst::getRoundLimitDegreeForce() const {
    return mRoundLimitDegreeForce;
}
float PlayerConst::getRoundLimitDegreeForceFast() const {
    return mRoundLimitDegreeForceFast;
}
int PlayerConst::getRoundAccelFrameForceFast() const {
    return mRoundAccelFrameForceFast;
}
float PlayerConst::getRoundLimitDegreeMin() const {
    return mRoundLimitDegreeMin;
}
float PlayerConst::getRoundLimitDegree() const {
    return mRoundLimitDegree;
}
int PlayerConst::getIceAccelFrame() const {
    return mIceAccelFrame;
}
int PlayerConst::getIceBrakeFrame() const {
    return mIceBrakeFrame;
}
int PlayerConst::getIceBrakeFrameHigh() const {
    return mIceBrakeFrameHigh;
}
int PlayerConst::getIceBrakeFrameWall() const {
    return mIceBrakeFrameWall;
}
int PlayerConst::getIceRoundAccelFrame() const {
    return mIceRoundAccelFrame;
}
int PlayerConst::getIceRoundAccelFrameFast() const {
    return mIceRoundAccelFrameFast;
}
int PlayerConst::getIceRoundBrakeFrame() const {
    return mIceRoundBrakeFrame;
}
float PlayerConst::getIceRoundFastDegree() const {
    return mIceRoundFastDegree;
}
float PlayerConst::getIceRoundMinDegree() const {
    return mIceRoundMinDegree;
}
float PlayerConst::getIceRoundLimitDegree() const {
    return mIceRoundLimitDegree;
}
float PlayerConst::getHillAddSpeed() const {
    return mHillAddSpeed;
}
float PlayerConst::getHillSubSpeed() const {
    return mHillSubSpeed;
}
int PlayerConst::getHillAccelAddFrame() const {
    return mHillAccelAddFrame;
}
int PlayerConst::getHillAccelSubFrame() const {
    return mHillAccelSubFrame;
}
float PlayerConst::getHillAccelSubAngleMin() const {
    return mHillAccelSubAngleMin;
}
float PlayerConst::getHillAccelSubAngleMax() const {
    return mHillAccelSubAngleMax;
}
float PlayerConst::getStandAngleMin() const {
    return mStandAngleMin;
}
float PlayerConst::getStandAngleMax() const {
    return mStandAngleMax;
}
float PlayerConst::getHillAngleSpeedMin() const {
    return mHillAngleSpeedMin;
}
float PlayerConst::getHillAngleSpeedMax() const {
    return mHillAngleSpeedMax;
}
int PlayerConst::getSpinCapThrowFrame() const {
    return mSpinCapThrowFrame;
}
int PlayerConst::getSpinCapThrowFrameAir() const {
    return mSpinCapThrowFrameAir;
}
int PlayerConst::getSpinCapThrowFrameSwim() const {
    return mSpinCapThrowFrameSwim;
}
int PlayerConst::getSpinCapThrowFrameSwing() const {
    return mSpinCapThrowFrameSwing;
}
int PlayerConst::getSpinCapThrowFrameContinuous() const {
    return mSpinCapThrowFrameContinuous;
}
int PlayerConst::getSpinAttackFrame() const {
    return mSpinAttackFrame;
}
int PlayerConst::getSpinBrakeFrame() const {
    return mSpinBrakeFrame;
}
float PlayerConst::getSpinAirJumpPower() const {
    return mSpinAirJumpPower;
}
float PlayerConst::getSpinAirSpeedMax() const {
    return mSpinAirSpeedMax;
}
float PlayerConst::getGravitySpinAir() const {
    return mGravitySpinAir;
}
float PlayerConst::getSlerpQuatRateSpinAir() const {
    return mSlerpQuatRateSpinAir;
}
float PlayerConst::getSpinBrakeRate() const {
    return mSpinBrakeRate;
}
float PlayerConst::getSpinBrakeSideAccel() const {
    return mSpinBrakeSideAccel;
}
float PlayerConst::getSpinBrakeSideBrakeRate() const {
    return mSpinBrakeSideBrakeRate;
}
float PlayerConst::getSpinBrakeSideMaxSpeedRate() const {
    return mSpinBrakeSideMaxSpeedRate;
}
float PlayerConst::getSpinRoundLimitDegree() const {
    return mSpinRoundLimitDegree;
}
float PlayerConst::getDamageFireJumpPower1st() const {
    return mDamageFireJumpPower1st;
}
float PlayerConst::getDamageFireJumpPower2nd() const {
    return mDamageFireJumpPower2nd;
}
float PlayerConst::getDamageFireJumpMoveSpeed() const {
    return mDamageFireJumpMoveSpeed;
}
float PlayerConst::getDamageFireCeilHitSpeed() const {
    return mDamageFireCeilHitSpeed;
}
float PlayerConst::getDamageFireGravity() const {
    return mDamageFireGravity;
}
int PlayerConst::getDamageFireNoGravityFrame() const {
    return mDamageFireNoGravityFrame;
}
int PlayerConst::getDamageFireRunTime() const {
    return mDamageFireRunTime;
}
float PlayerConst::getDamageFireRunSpeed() const {
    return mDamageFireRunSpeed;
}
float PlayerConst::getDamageFireRunBrakeFrame() const {
    return mDamageFireRunBrakeFrame;
}
int PlayerConst::getSandSinkDeadTime() const {
    return mSandSinkDeadTime;
}
int PlayerConst::getSandSinkBrakeHeightH() const {
    return mSandSinkBrakeHeightH;
}
int PlayerConst::getSandSinkBrakeHeightV() const {
    return mSandSinkBrakeHeightV;
}
float PlayerConst::getSandSinkHeight() const {
    return mSandSinkHeight;
}
float PlayerConst::getSandSinkCapThrow() const {
    return mSandSinkCapThrow;
}
float PlayerConst::getSandSinkBrakeMinH() const {
    return mSandSinkBrakeMinH;
}
float PlayerConst::getSandSinkBrakeMaxH() const {
    return mSandSinkBrakeMaxH;
}
float PlayerConst::getSandSinkBrakeMinV() const {
    return mSandSinkBrakeMinV;
}
float PlayerConst::getSandSinkBrakeMaxV() const {
    return mSandSinkBrakeMaxV;
}
float PlayerConst::getSlopeSlideAngleStart() const {
    return mSlopeSlideAngleStart;
}
float PlayerConst::getSlopeSlideAngleEnd() const {
    return mSlopeSlideAngleEnd;
}
float PlayerConst::getSlopeSlideAccel() const {
    return mSlopeSlideAccel;
}
float PlayerConst::getSlopeSlideBrake() const {
    return mSlopeSlideBrake;
}
float PlayerConst::getSlopeSlideMaxSpeed() const {
    return mSlopeSlideMaxSpeed;
}
float PlayerConst::getSlopeSlideSpeedEnd() const {
    return mSlopeSlideSpeedEnd;
}
float PlayerConst::getSlopeSlideSideAccel() const {
    return mSlopeSlideSideAccel;
}
float PlayerConst::getSlopeSlideSideBrake() const {
    return mSlopeSlideSideBrake;
}
float PlayerConst::getSlopeSlideSideMaxSpeed() const {
    return mSlopeSlideSideMaxSpeed;
}
float PlayerConst::getSlopeTurnDegree() const {
    return mSlopeTurnDegree;
}
int PlayerConst::getSlideInvalidFrame() const {
    return mSlideInvalidFrame;
}
int PlayerConst::getSlopeForceFrame() const {
    return mSlopeForceFrame;
}
float PlayerConst::getSlopeSlideForceSideAccel() const {
    return mSlopeSlideForceSideAccel;
}
float PlayerConst::getSlopeSlideForceSideBrake() const {
    return mSlopeSlideForceSideBrake;
}
float PlayerConst::getSlopeSlideForceSideMaxSpeed() const {
    return mSlopeSlideForceSideMaxSpeed;
}
float PlayerConst::getSlopeSlideForceTurnDegree() const {
    return mSlopeSlideForceTurnDegree;
}
float PlayerConst::getSlopeRollingSpeedStart() const {
    return mSlopeRollingSpeedStart;
}
float PlayerConst::getSlopeRollingSpeedBoost() const {
    return mSlopeRollingSpeedBoost;
}
float PlayerConst::getSlopeRollingMaxSpeed() const {
    return mSlopeRollingMaxSpeed;
}
int PlayerConst::getSlopeRollingFrameMinBoost() const {
    return mSlopeRollingFrameMinBoost;
}
int PlayerConst::getSlopeRollingFrameMin() const {
    return mSlopeRollingFrameMin;
}
float PlayerConst::getSlopeRollingStartJumpPower() const {
    return mSlopeRollingStartJumpPower;
}
float PlayerConst::getSlopeRollingStartSlideSpeed() const {
    return mSlopeRollingStartSlideSpeed;
}
float PlayerConst::getSlopeRollingAccel() const {
    return mSlopeRollingAccel;
}
float PlayerConst::getSlopeRollingBrake() const {
    return mSlopeRollingBrake;
}
float PlayerConst::getSlopeRollingAgainst() const {
    return mSlopeRollingAgainst;
}
float PlayerConst::getSlopeRollingAnglePowerMax() const {
    return mSlopeRollingAnglePowerMax;
}
float PlayerConst::getSlopeRollingSpeedEnd() const {
    return mSlopeRollingSpeedEnd;
}
float PlayerConst::getSlopeRollingSideAccel() const {
    return mSlopeRollingSideAccel;
}
float PlayerConst::getSlopeRollingSideBrake() const {
    return mSlopeRollingSideBrake;
}
float PlayerConst::getSlopeRollingSideMaxSpeed() const {
    return mSlopeRollingSideMaxSpeed;
}
int PlayerConst::getSlopeRollingUnRollFrame() const {
    return mSlopeRollingUnRollFrame;
}
float PlayerConst::getSlopeRollingEndBrake() const {
    return mSlopeRollingEndBrake;
}
float PlayerConst::getSlopeRollingEndBrakeEndSpeed() const {
    return mSlopeRollingEndBrakeEndSpeed;
}
float PlayerConst::getSlopeRollingReStartAccel() const {
    return mSlopeRollingReStartAccel;
}
float PlayerConst::getSlopeRollingReStartMaxAdd() const {
    return mSlopeRollingReStartMaxAdd;
}
int PlayerConst::getSlopeRollingReStartInterval() const {
    return mSlopeRollingReStartinterval;
}
int PlayerConst::getSlopeRollingReStartSwing() const {
    return mSlopeRollingReStartSwing;
}
int PlayerConst::getSlopeRollingReStartCharge() const {
    return mSlopeRollingReStartCharge;
}
int PlayerConst::getSlopeRollingReStartForce() const {
    return mSlopeRollingReStartForce;
}
float PlayerConst::getSlopeRollingAccelOnSkate() const {
    return mSlopeRollingAccelOnSkate;
}
float PlayerConst::getSlopeRollingSideAccelOnSkate() const {
    return mSlopeRollingSideAccelOnSkate;
}
float PlayerConst::getSlopeRollingBrakeOnSkate() const {
    return mSlopeRollingBrakeOnSkate;
}
int PlayerConst::getExtendFrame() const {
    return mExtendFrame;
}
float PlayerConst::getJumpInertiaRate() const {
    return mJumpInertiaRate;
}
float PlayerConst::getJumpPowerMin() const {
    return mJumpPowerMin;
}
float PlayerConst::getJumpPowerMax() const {
    return mJumpPowerMax;
}
float PlayerConst::getJumpGravity() const {
    return mJumpGravity;
}
float PlayerConst::getJumpBaseSpeedMax() const {
    return mJumpBaseSpeedMax;
}
float PlayerConst::getJumpMoveSpeedMin() const {
    return mJumpMoveSpeedMin;
}
float PlayerConst::getJumpMoveSpeedMax() const {
    return mJumpMoveSpeedMax;
}
float PlayerConst::getJumpAccelFront() const {
    return mJumpAccelFront;
}
float PlayerConst::getJumpAccelBack() const {
    return mJumpAccelBack;
}
float PlayerConst::getJumpAccelTurn() const {
    return mJumpAccelTurn;
}
float PlayerConst::getJumpTurnAngleStart() const {
    return mJumpTurnAngleStart;
}
float PlayerConst::getJumpTurnAngleLimit() const {
    return mJumpTurnAngleLimit;
}
float PlayerConst::getJumpTurnAngleFast() const {
    return mJumpTurnAngleFast;
}
float PlayerConst::getJumpTurnAngleFastLimit() const {
    return mJumpTurnAngleFastLimit;
}
int PlayerConst::getJumpTurnAccelFrame() const {
    return mJumpTurnAccelFrame;
}
int PlayerConst::getJumpTurnAccelFrameFast() const {
    return mJumpTurnAccelFrameFast;
}
int PlayerConst::getJumpTurnBrakeFrame() const {
    return mJumpTurnBrakeFrame;
}
float PlayerConst::getTrampleGravity() const {
    return mTrampleGravity;
}
float PlayerConst::getTrampleJumpPower() const {
    return mTrampleJumpPower;
}
float PlayerConst::getTrampleHighGravity() const {
    return mTrampleHighGravity;
}
float PlayerConst::getTrampleHighJumpPower() const {
    return mTrampleHighJumpPower;
}
float PlayerConst::getTrampleGravity2D() const {
    return mTrampleGravity2D;
}
float PlayerConst::getTrampleJumpPower2D() const {
    return mTrampleJumpPower2D;
}
float PlayerConst::getTrampleHighGravity2D() const {
    return mTrampleHighGravity2D;
}
float PlayerConst::getTrampleHighJumpPower2D() const {
    return mTrampleHighJumpPower2D;
}
float PlayerConst::getTrampleHipDropGravity() const {
    return mTrampleHipDropGravity;
}
float PlayerConst::getTrampleHipDropJumpPower() const {
    return mTrampleHipDropJumpPower;
}
float PlayerConst::getTrampleRisingBrakeVelH() const {
    return mTrampleRisingBrakeVelH;
}
float PlayerConst::getTrampleJumpCodePower() const {
    return mTrampleJumpCodePower;
}
float PlayerConst::getTrampleJumpCodePowerSmall() const {
    return mTrampleJumpCodePowerSmall;
}
float PlayerConst::getCapLeapFrogJumpGravity() const {
    return mCapLeapFrogJumpGravity;
}
float PlayerConst::getCapLeapFrogJumpPower() const {
    return mCapLeapFrogJumpPower;
}
float PlayerConst::getCapLeapFrogJumpPowerAir() const {
    return mCapLeapFrogJumpPowerAir;
}
float PlayerConst::getObjLeapFrogJumpPower() const {
    return mObjLeapFrogJumpPower;
}
float PlayerConst::getObjLeapFrogJumpPowerHigh() const {
    return mObjLeapFrogJumpPowerHigh;
}
float PlayerConst::getCapHeadSpringJumpGravity() const {
    return mCapHeadSpringJumpGravity;
}
float PlayerConst::getCapHeadSpringJumpPower() const {
    return mCapHeadSpringJumpPower;
}
float PlayerConst::getCapHeadSpringJumpGravityHigh() const {
    return mCapHeadSpringJumpGravityHigh;
}
float PlayerConst::getCapHeadSpringJumpPowerHigh() const {
    return mCapHeadSpringJumpPowerHigh;
}
float PlayerConst::getCapHeadSpringSpeedMax() const {
    return mCapHeadSpringSpeedMax;
}
int PlayerConst::getContinuousJumpPreInputFrame() const {
    return mContinuousJumpPreInputFrame;
}
int PlayerConst::getContinuousJumpCount() const {
    return mContinuousJumpCount;
}
int PlayerConst::getContinuousJumpTimer() const {
    return mContinuousJumpTimer;
}
float PlayerConst::getContinuousJumpPowerMin() const {
    return mContinuousJumpPowerMin;
}
float PlayerConst::getJumpPowerMax2nd() const {
    return mJumpPowerMax2nd;
}
float PlayerConst::getJumpGravity2nd() const {
    return mJumpGravity2nd;
}
float PlayerConst::getJumpPowerMax3rd() const {
    return mJumpPowerMax3rd;
}
float PlayerConst::getJumpGravity3rd() const {
    return mJumpGravity3rd;
}
float PlayerConst::getSpinFlowerJumpGravity() const {
    return mSpinFlowerJumpGravity;
}
float PlayerConst::getSpinFlowerJumpFallSpeedMax() const {
    return mSpinFlowerJumpFallSpeedMax;
}
float PlayerConst::getSpinFlowerJumpMovePower() const {
    return mSpinFlowerJumpMovePower;
}
float PlayerConst::getSpinFlowerJumpVelMax() const {
    return mSpinFlowerJumpVelMax;
}
int PlayerConst::getSpinFlowerJumpStayFrame() const {
    return mSpinFlowerJumpStayFrame;
}
float PlayerConst::getSpinFlowerJumpStaySpeedMax() const {
    return mSpinFlowerJumpStaySpeedMax;
}
float PlayerConst::getSpinFlowerJumpNoInputBrake() const {
    return mSpinFlowerJumpNoInputBrake;
}
float PlayerConst::getSpinFlowerJumpDownFallInitSpeed() const {
    return mSpinFlowerJumpDownFallInitSpeed;
}
float PlayerConst::getSpinFlowerJumpDownFallPower() const {
    return mSpinFlowerJumpDownFallPower;
}
float PlayerConst::getSpinFlowerJumpDownFallSpeedMax() const {
    return mSpinFlowerJumpDownFallSpeedMax;
}
float PlayerConst::getJumpGravityForceRun() const {
    return mJumpGravityForceRun;
}
float PlayerConst::getJumpPowerForceRun() const {
    return mJumpPowerForceRun;
}
float PlayerConst::getCapCatchPopPower() const {
    return mCapCatchPopPower;
}
float PlayerConst::getCapCatchPopGravity() const {
    return mCapCatchPopGravity;
}
float PlayerConst::getSquatJumpGravity() const {
    return mSquatJumpGravity;
}
float PlayerConst::getSquatJumpPower() const {
    return mSquatJumpPower;
}
float PlayerConst::getSquatJumpBackPower() const {
    return mSquatJumpBackPower;
}
float PlayerConst::getSquatJumpMovePowerFront() const {
    return mSquatJumpMovePowerFront;
}
float PlayerConst::getSquatJumpMovePowerSide() const {
    return mSquatJumpMovePowerSide;
}
float PlayerConst::getSquatJumpMoveSpeedMax() const {
    return mSquatJumpMoveSpeedMax;
}
float PlayerConst::getTurnJumpGravity() const {
    return mTurnJumpGravity;
}
float PlayerConst::getTurnJumpPower() const {
    return mTurnJumpPower;
}
float PlayerConst::getTurnJumpVelH() const {
    return mTurnJumpVelH;
}
float PlayerConst::getTurnJumpBrake() const {
    return mTurnJumpBrake;
}
float PlayerConst::getTurnJumpAccel() const {
    return mTurnJumpAccel;
}
float PlayerConst::getTurnJumpSideAccel() const {
    return mTurnJumpSideAccel;
}
float PlayerConst::getLongJumpAccel() const {
    return mLongJumpAccel;
}
float PlayerConst::getLongJumpBrake() const {
    return mLongJumpBrake;
}
float PlayerConst::getLongJumpSideAccel() const {
    return mLongJumpSideAccel;
}
float PlayerConst::getLongJumpGravity() const {
    return mLongJumpGravity;
}
float PlayerConst::getLongJumpJumpPow() const {
    return mLongJumpJumpPow;
}
float PlayerConst::getLongJumpMovePow() const {
    return mLongJumpMovePow;
}
float PlayerConst::getLongJumpInitSpeed() const {
    return mLongJumpInitSpeed;
}
float PlayerConst::getLongJumpSpeed() const {
    return mLongJumpSpeed;
}
float PlayerConst::getLongJumpSpeedMin() const {
    return mLongJumpSpeedMin;
}
int PlayerConst::getContinuousLongJumpCount() const {
    return mContinuousLongJumpCount;
}
int PlayerConst::getContinuousLongJumpTimer() const {
    return mContinuousLongJumpTimer;
}
float PlayerConst::getGravityAir() const {
    return mGravityAir;
}
float PlayerConst::getFrictionAir() const {
    return mFrictionAir;
}
float PlayerConst::getFallSpeedMax() const {
    return mFallSpeedMax;
}
float PlayerConst::getLongFallDistance() const {
    return mLongFallDistance;
}
float PlayerConst::getFallWallScaleVelocity() const {
    return mFallWallScaleVelocity;
}
int PlayerConst::getDownFallFrameMin() const {
    return mDownFallFrameMin;
}
float PlayerConst::getGravityWallSlide() const {
    return mGravityWallSlide;
}
float PlayerConst::getWallHeightLowLimit() const {
    return mWallHeightLowLimit;
}
float PlayerConst::getWallKeepDegree() const {
    return mWallKeepDegree;
}
int PlayerConst::getWallKeepFrame() const {
    return mWallKeepFrame;
}
float PlayerConst::getWallJumpGravity() const {
    return mWallJumpGravity;
}
float PlayerConst::getWallJumpHSpeed() const {
    return mWallJumpHSpeed;
}
float PlayerConst::getWallJumpPower() const {
    return mWallJumpPower;
}
int PlayerConst::getWallJumpInvalidateInputFrame() const {
    return mWallJumpInvalidateInputFrame;
}
int PlayerConst::getWallInhibitAfterPunch() const {
    return mWallInhibitAfterPunch;
}
float PlayerConst::getWallFollowAngleH() const {
    return mWallFollowAngleH;
}
float PlayerConst::getWallFollowAngleV() const {
    return mWallFollowAngleV;
}
float PlayerConst::getWallCatchDegree() const {
    return mWallCatchDegree;
}
float PlayerConst::getWallCatchHeightEdgeTop() const {
    return mWallCatchHeightEdgeTop;
}
float PlayerConst::getWallCatchHeightBottom() const {
    return mWallCatchHeightBottom;
}
float PlayerConst::getWallCatchKeepDegree() const {
    return mWallCatchKeepDegree;
}
float PlayerConst::getWallCatchMoveDegree() const {
    return mWallCatchMoveDegree;
}
float PlayerConst::getWallCatchMoveSpeed() const {
    return mWallCatchMoveSpeed;
}
float PlayerConst::getWallCatchMoveHeightRange() const {
    return mWallCatchMoveHeightRange;
}
int PlayerConst::getWallCatchMoveInterpolate() const {
    return mWallCatchMoveinterpolate;
}
int PlayerConst::getWallCatchMoveFrame() const {
    return mWallCatchMoveFrame;
}
int PlayerConst::getWallCatchMoveFrameFast() const {
    return mWallCatchMoveFrameFast;
}
int PlayerConst::getWallCatchMoveFrameSwing() const {
    return mWallCatchMoveFrameSwing;
}
float PlayerConst::getWallCatchInputRepeatAngle() const {
    return mWallCatchInputRepeatAngle;
}
float PlayerConst::getWallClimbDegree() const {
    return mWallClimbDegree;
}
int PlayerConst::getWallClimbJumpStartFrame() const {
    return mWallClimbJumpStartFrame;
}
int PlayerConst::getWallClimbJumpEndFrame() const {
    return mWallClimbJumpEndFrame;
}
int PlayerConst::getWallClimbStartFrame() const {
    return mWallClimbStartFrame;
}
float PlayerConst::getWallClimbGravity() const {
    return mWallClimbGravity;
}
float PlayerConst::getWallFallJumpSpeed() const {
    return mWallFallJumpSpeed;
}
float PlayerConst::getWallClimbJumpSpeedV() const {
    return mWallClimbJumpSpeedV;
}
float PlayerConst::getWallClimbJumpSpeedH() const {
    return mWallClimbJumpSpeedH;
}
float PlayerConst::getWallClimbJumpGravity() const {
    return mWallClimbJumpGravity;
}
int PlayerConst::getWallClimbJumpInvalidFrame() const {
    return mWallClimbJumpInvalidFrame;
}
float PlayerConst::getWallCatchHipLocalOffset() const {
    return mWallCatchHipLocalOffset;
}
float PlayerConst::getWallCatchHipStability() const {
    return mWallCatchHipStability;
}
float PlayerConst::getWallCatchHipFriction() const {
    return mWallCatchHipFriction;
}
float PlayerConst::getWallCatchHipLimitDegree() const {
    return mWallCatchHipLimitDegree;
}
float PlayerConst::getWallCatchStainAreaOffset() const {
    return mWallCatchStainAreaOffset;
}
float PlayerConst::getGrabCeilRange() const {
    return mGrabCeilRange;
}
float PlayerConst::getGrabCeilBodyRadius() const {
    return mGrabCeilBodyRadius;
}
float PlayerConst::getGrabCeilLeaveSpeedMin() const {
    return mGrabCeilLeaveSpeedMin;
}
float PlayerConst::getGrabCeilLeavePopPower() const {
    return mGrabCeilLeavePopPower;
}
float PlayerConst::getGrabCeilLeavePopGravity() const {
    return mGrabCeilLeavePopGravity;
}
float PlayerConst::getGrabCeilSwingStartOffset() const {
    return mGrabCeilSwingStartOffset;
}
float PlayerConst::getGrabCeilReverseInputBorder() const {
    return mGrabCeilReverseInputBorder;
}
float PlayerConst::getGrabCeilInputPowerBorder() const {
    return mGrabCeilInputPowerBorder;
}
float PlayerConst::getGrabCeilSwingWaitEnergy() const {
    return mGrabCeilSwingWaitEnergy;
}
float PlayerConst::getGrabCeilEnableJumpEnergy() const {
    return mGrabCeilEnableJumpEnergy;
}
float PlayerConst::getGrabCeilEnableJumpEnergyMax() const {
    return mGrabCeilEnableJumpEnergyMax;
}
float PlayerConst::getGrabCeilJumpForceAngle() const {
    return mGrabCeilJumpForceAngle;
}
float PlayerConst::getGrabCeilJumpPower() const {
    return mGrabCeilJumpPower;
}
float PlayerConst::getGrabCeilJumpMoveMin() const {
    return mGrabCeilJumpMoveMin;
}
float PlayerConst::getGrabCeilJumpMoveMax() const {
    return mGrabCeilJumpMoveMax;
}
float PlayerConst::getGrabCeilJumpGravity() const {
    return mGrabCeilJumpGravity;
}
int PlayerConst::getGrabCeilJumpInvalidFrame() const {
    return mGrabCeilJumpInvalidFrame;
}
int PlayerConst::getGrabCeilEnableNextFrame() const {
    return mGrabCeilEnableNextFrame;
}
int PlayerConst::getGrabCeilEnableFallSnapFrame() const {
    return mGrabCeilEnableFallSnapFrame;
}
int PlayerConst::getPoleClimbPreInputSwing() const {
    return mPoleClimbPreInputSwing;
}
float PlayerConst::getPoleClimbInputRepeatAngle() const {
    return mPoleClimbInputRepeatAngle;
}
float PlayerConst::getPoleClimbInputDegreeMove() const {
    return mPoleClimbInputDegreeMove;
}
float PlayerConst::getPoleClimbCatchRange() const {
    return mPoleClimbCatchRange;
}
float PlayerConst::getPoleClimbCatchRangeMin() const {
    return mPoleClimbCatchRangeMin;
}
float PlayerConst::getPoleClimbCatchRangeMax() const {
    return mPoleClimbCatchRangeMax;
}
float PlayerConst::getPoleClimbJointAngleMin() const {
    return mPoleClimbJointAngleMin;
}
float PlayerConst::getPoleClimbJointAngleMax() const {
    return mPoleClimbJointAngleMax;
}
float PlayerConst::getPoleClimbJointRangeMin() const {
    return mPoleClimbJointRangeMin;
}
float PlayerConst::getPoleClimbJointRangeMax() const {
    return mPoleClimbJointRangeMax;
}
float PlayerConst::getPoleClimbMoveWallDegree() const {
    return mPoleClimbMoveWallDegree;
}
float PlayerConst::getPoleClimbUpMargine() const {
    return mPoleClimbUpMargine;
}
float PlayerConst::getPoleClimbUpSpeed() const {
    return mPoleClimbUpSpeed;
}
int PlayerConst::getPoleClimbUpFrame() const {
    return mPoleClimbUpFrame;
}
int PlayerConst::getPoleClimbUpFrameFast() const {
    return mPoleClimbUpFrameFast;
}
int PlayerConst::getPoleClimbUpFrameSwing() const {
    return mPoleClimbUpFrameSwing;
}
float PlayerConst::getPoleClimbDownSpeed() const {
    return mPoleClimbDownSpeed;
}
float PlayerConst::getPoleClimbDownSpeedFast() const {
    return mPoleClimbDownSpeedFast;
}
float PlayerConst::getPoleClimbDownSpeedSwing() const {
    return mPoleClimbDownSpeedSwing;
}
int PlayerConst::getPoleClimbDownFrame() const {
    return mPoleClimbDownFrame;
}
int PlayerConst::getPoleClimbDownKeepTime() const {
    return mPoleClimbDownKeepTime;
}
float PlayerConst::getPoleClimbTurnDist() const {
    return mPoleClimbTurnDist;
}
int PlayerConst::getPoleClimbTurnFrame() const {
    return mPoleClimbTurnFrame;
}
int PlayerConst::getPoleClimbTurnStopFrame() const {
    return mPoleClimbTurnStopFrame;
}
int PlayerConst::getPoleTopStartFrame() const {
    return mPoleTopStartFrame;
}
int PlayerConst::getPoleTopEndFrame() const {
    return mPoleTopEndFrame;
}
float PlayerConst::getPoleTopTurnSpeed() const {
    return mPoleTopTurnSpeed;
}
float PlayerConst::getPoleTopEndUnderOffsetY() const {
    return mPoleTopEndUnderOffsetY;
}
int PlayerConst::getGroundSpinFrame() const {
    return mGroundSpinFrame;
}
float PlayerConst::getGroundSpinMoveSpeedMax() const {
    return mGroundSpinMoveSpeedMax;
}
float PlayerConst::getGroundSpinAccelRate() const {
    return mGroundSpinAccelRate;
}
float PlayerConst::getGroundSpinBrakeRate() const {
    return mGroundSpinBrakeRate;
}
float PlayerConst::getSpinJumpGravity() const {
    return mSpinJumpGravity;
}
float PlayerConst::getSpinJumpPower() const {
    return mSpinJumpPower;
}
float PlayerConst::getSpinJumpMoveSpeedMax() const {
    return mSpinJumpMoveSpeedMax;
}
float PlayerConst::getSpinJumpDownFallInitSpeed() const {
    return mSpinJumpDownFallInitSpeed;
}
float PlayerConst::getSpinJumpDownFallPower() const {
    return mSpinJumpDownFallPower;
}
float PlayerConst::getSpinJumpDownFallSpeedMax() const {
    return mSpinJumpDownFallSpeedMax;
}
float PlayerConst::getSquatBrakeEndSpeed() const {
    return mSquatBrakeEndSpeed;
}
float PlayerConst::getSquatAccelRate() const {
    return mSquatAccelRate;
}
float PlayerConst::getSquatBrakeRate() const {
    return mSquatBrakeRate;
}
float PlayerConst::getSquatBrakeRateOnSkate() const {
    return mSquatBrakeRateOnSkate;
}
float PlayerConst::getSquatBrakeSideAccel() const {
    return mSquatBrakeSideAccel;
}
float PlayerConst::getSquatBrakeSideRate() const {
    return mSquatBrakeSideRate;
}
float PlayerConst::getSquatBrakeSideAccelOnSkate() const {
    return mSquatBrakeSideAccelOnSkate;
}
float PlayerConst::getSquatBrakeSideRateOnSkate() const {
    return mSquatBrakeSideRateOnSkate;
}
float PlayerConst::getSquatBrakeSideMaxSpeedRate() const {
    return mSquatBrakeSideMaxSpeedRate;
}
float PlayerConst::getSquatWalkSpeed() const {
    return mSquatWalkSpeed;
}
float PlayerConst::getSquatWalkTurnSpeed() const {
    return mSquatWalkTurnSpeed;
}
int PlayerConst::getSquatWalkTurnFrame() const {
    return mSquatWalkTurnFrame;
}
float PlayerConst::getSquatJumpCeilSlideSpeed2D() const {
    return mSquatJumpCeilSlideSpeed2D;
}
float PlayerConst::getHipDropSpeed() const {
    return mHipDropSpeed;
}
float PlayerConst::getHipDropGravity() const {
    return mHipDropGravity;
}
float PlayerConst::getHipDropSpeedMax() const {
    return mHipDropSpeedMax;
}
int PlayerConst::getHipDropLandCancelFrame() const {
    return mHipDropLandCancelFrame;
}
float PlayerConst::getHipDropHeight() const {
    return mHipDropHeight;
}
int PlayerConst::getHipDropMsgInterval() const {
    return mHipDropMsginterval;
}
float PlayerConst::getJumpHipDropPower() const {
    return mJumpHipDropPower;
}
int PlayerConst::getJumpHipDropPermitBeginFrame() const {
    return mJumpHipDropPermitBeginFrame;
}
int PlayerConst::getJumpHipDropPermitEndFrame() const {
    return mJumpHipDropPermitEndFrame;
}
float PlayerConst::getHeadSlidingSpeed() const {
    return mHeadSlidingSpeed;
}
float PlayerConst::getHeadSlidingSpeedMin() const {
    return mHeadSlidingSpeedMin;
}
float PlayerConst::getHeadSlidingBrake() const {
    return mHeadSlidingBrake;
}
float PlayerConst::getHeadSlidingSideAccel() const {
    return mHeadSlidingSideAccel;
}
float PlayerConst::getHeadSlidingJump() const {
    return mHeadSlidingJump;
}
float PlayerConst::getHeadSlidingGravityAir() const {
    return mHeadSlidingGravityAir;
}
float PlayerConst::getSwimCenterOffset() const {
    return mSwimCenterOffset;
}
float PlayerConst::getSwimWallCatchOffset() const {
    return mSwimWallCatchOffset;
}
float PlayerConst::getSwimRisePower() const {
    return mSwimRisePower;
}
float PlayerConst::getSwimRiseSpeedMax() const {
    return mSwimRiseSpeedMax;
}
int PlayerConst::getSwimRiseFrame() const {
    return mSwimRiseFrame;
}
float PlayerConst::getSwimGravity() const {
    return mSwimGravity;
}
float PlayerConst::getSwimGravityWalk() const {
    return mSwimGravityWalk;
}
float PlayerConst::getSwimFallSpeedMax() const {
    return mSwimFallSpeedMax;
}
float PlayerConst::getSwimFloorAccelH() const {
    return mSwimFloorAccelH;
}
float PlayerConst::getSwimFloorSpeedMaxH() const {
    return mSwimFloorSpeedMaxH;
}
float PlayerConst::getSwimHighAccelH() const {
    return mSwimHighAccelH;
}
float PlayerConst::getSwimHighSpeedMaxH() const {
    return mSwimHighSpeedMaxH;
}
float PlayerConst::getSwimLowAccelH() const {
    return mSwimLowAccelH;
}
float PlayerConst::getSwimLowSpeedMaxH() const {
    return mSwimLowSpeedMaxH;
}
float PlayerConst::getSwimBrakeRateH() const {
    return mSwimBrakeRateH;
}
float PlayerConst::getSwimWallHitSpeedMinH() const {
    return mSwimWallHitSpeedMinH;
}
int PlayerConst::getSwimHighAccelPermitFrame() const {
    return mSwimHighAccelPermitFrame;
}
float PlayerConst::getSwimFlowFieldBlend() const {
    return mSwimFlowFieldBlend;
}
float PlayerConst::getSwimWalkAnimMinRate() const {
    return mSwimWalkAnimMinRate;
}
float PlayerConst::getSwimWalkAnimMaxRate() const {
    return mSwimWalkAnimMaxRate;
}
float PlayerConst::getSwimWalkMaxSpeed() const {
    return mSwimWalkMaxSpeed;
}
float PlayerConst::getSwimSpinCapUpPower() const {
    return mSwimSpinCapUpPower;
}
float PlayerConst::getSwimSpinCapUpSpeedMax() const {
    return mSwimSpinCapUpSpeedMax;
}
float PlayerConst::getSwimRotStartAngle() const {
    return mSwimRotStartAngle;
}
float PlayerConst::getSwimRotFastAngle() const {
    return mSwimRotFastAngle;
}
int PlayerConst::getSwimRotAccelFrame() const {
    return mSwimRotAccelFrame;
}
int PlayerConst::getSwimRotAccelFrameFast() const {
    return mSwimRotAccelFrameFast;
}
int PlayerConst::getSwimRotBrakeFrame() const {
    return mSwimRotBrakeFrame;
}
float PlayerConst::getSwimRotSpeedChangeStart() const {
    return mSwimRotSpeedChangeStart;
}
float PlayerConst::getSwimRotSpeedForward() const {
    return mSwimRotSpeedForward;
}
float PlayerConst::getSwimRotSpeedMax() const {
    return mSwimRotSpeedMax;
}
float PlayerConst::getSwimSurfaceAccelH() const {
    return mSwimSurfaceAccelH;
}
float PlayerConst::getSwimSurfaceSpeedMaxH() const {
    return mSwimSurfaceSpeedMaxH;
}
int PlayerConst::getSwimSurfaceSpinCapFrame() const {
    return mSwimSurfaceSpinCapFrame;
}
float PlayerConst::getSwimSurfaceSpinCapSpeedMaxH() const {
    return mSwimSurfaceSpinCapSpeedMaxH;
}
float PlayerConst::getSwimSurfaceStartDist() const {
    return mSwimSurfaceStartDist;
}
float PlayerConst::getSwimSurfaceEndDist() const {
    return mSwimSurfaceEndDist;
}
float PlayerConst::getSwimSurfaceGravity() const {
    return mSwimSurfaceGravity;
}
float PlayerConst::getSwimSurfaceBaseHeight() const {
    return mSwimSurfaceBaseHeight;
}
float PlayerConst::getSwimSurfaceSpring() const {
    return mSwimSurfaceSpring;
}
float PlayerConst::getSwimSurfaceDamper() const {
    return mSwimSurfaceDamper;
}
int PlayerConst::getSwimSurfaceDamperStart() const {
    return mSwimSurfaceDamperStart;
}
int PlayerConst::getSwimSurfaceDamperFrame() const {
    return mSwimSurfaceDamperFrame;
}
float PlayerConst::getSwimSurfaceEnableJumpHeight() const {
    return mSwimSurfaceEnableJumpHeight;
}
int PlayerConst::getSwimSurfacePreInputJumpFrame() const {
    return mSwimSurfacePreInputJumpFrame;
}
float PlayerConst::getSwimSurfaceMoveSpring() const {
    return mSwimSurfaceMoveSpring;
}
float PlayerConst::getSwimSurfaceMoveDamper() const {
    return mSwimSurfaceMoveDamper;
}
float PlayerConst::getSwimSurfaceMoveBaseHeight() const {
    return mSwimSurfaceMoveBaseHeight;
}
float PlayerConst::getSwimRunSurfaceBaseHeight() const {
    return mSwimRunSurfaceBaseHeight;
}
float PlayerConst::getSwimRunSurfaceApproachRate() const {
    return mSwimRunSurfaceApproachRate;
}
float PlayerConst::getSwimRunSurfaceApproachLimit() const {
    return mSwimRunSurfaceApproachLimit;
}
float PlayerConst::getSwimRunSurfaceBrakeBorder() const {
    return mSwimRunSurfaceBrakeBorder;
}
float PlayerConst::getSwimRunSurfaceBrakeH() const {
    return mSwimRunSurfaceBrakeH;
}
float PlayerConst::getSwimRunSurfaceApproachBorderMax() const {
    return mSwimRunSurfaceApproachBorderMax;
}
float PlayerConst::getSwimRunSurfaceApproachBorderMin() const {
    return mSwimRunSurfaceApproachBorderMin;
}
float PlayerConst::getSwimRunSurfaceApproachRateMin() const {
    return mSwimRunSurfaceApproachRateMin;
}
float PlayerConst::getSwimFallInSpeed() const {
    return mSwimFallInSpeed;
}
float PlayerConst::getSwimFallInBrakeH() const {
    return mSwimFallInBrakeH;
}
float PlayerConst::getSwimFallInBrakeV() const {
    return mSwimFallInBrakeV;
}
float PlayerConst::getSwimHeadInBrakeH() const {
    return mSwimHeadInBrakeH;
}
float PlayerConst::getSwimHeadInBrakeV() const {
    return mSwimHeadInBrakeV;
}
float PlayerConst::getSwimHeadInRisePower() const {
    return mSwimHeadInRisePower;
}
float PlayerConst::getSwimHeadInRiseSpeedMax() const {
    return mSwimHeadInRiseSpeedMax;
}
float PlayerConst::getSwimHeadInSurfaceHeight() const {
    return mSwimHeadInSurfaceHeight;
}
int PlayerConst::getSwimFallInForceSurfaceFrame() const {
    return mSwimFallInForceSurfaceFrame;
}
int PlayerConst::getSwimFallInvalidJumpFrame() const {
    return mSwimFallInvalidJumpFrame;
}
float PlayerConst::getSwimDiveStartSpeed() const {
    return mSwimDiveStartSpeed;
}
float PlayerConst::getSwimDiveBrake() const {
    return mSwimDiveBrake;
}
float PlayerConst::getSwimDiveEndSpeed() const {
    return mSwimDiveEndSpeed;
}
int PlayerConst::getSwimDiveLandCount() const {
    return mSwimDiveLandCount;
}
int PlayerConst::getSwimDiveLandCancelFrame() const {
    return mSwimDiveLandCancelFrame;
}
int PlayerConst::getSwimDiveNoBrakeFrame() const {
    return mSwimDiveNoBrakeFrame;
}
int PlayerConst::getSwimDiveButtonValidFrame() const {
    return mSwimDiveButtonValidFrame;
}
int PlayerConst::getSwimDiveEndFrame() const {
    return mSwimDiveEndFrame;
}
float PlayerConst::getSwimDiveInBrakeH() const {
    return mSwimDiveInBrakeH;
}
float PlayerConst::getSwimDiveInBrakeV() const {
    return mSwimDiveInBrakeV;
}
float PlayerConst::getSwimDiveInRisePower() const {
    return mSwimDiveInRisePower;
}
float PlayerConst::getSwimDiveInRiseSpeedMax() const {
    return mSwimDiveInRiseSpeedMax;
}
float PlayerConst::getSwimDiveInSurfaceHeight() const {
    return mSwimDiveInSurfaceHeight;
}
int PlayerConst::getSwimDiveInKeepFrame() const {
    return mSwimDiveInKeepFrame;
}
int PlayerConst::getSwimHeadSlidingFrame() const {
    return mSwimHeadSlidingFrame;
}
int PlayerConst::getSwimHeadSlidingBrakeFrame() const {
    return mSwimHeadSlidingBrakeFrame;
}
float PlayerConst::getSwimHeadSlidingSpeed() const {
    return mSwimHeadSlidingSpeed;
}
float PlayerConst::getSwimHeadSlidingSpeedEnd() const {
    return mSwimHeadSlidingSpeedEnd;
}
float PlayerConst::getSwimHeadSlidingBrake() const {
    return mSwimHeadSlidingBrake;
}
float PlayerConst::getSwimHeadSlidingSideAccel() const {
    return mSwimHeadSlidingSideAccel;
}
float PlayerConst::getSwimHeadSlidingJump() const {
    return mSwimHeadSlidingJump;
}
float PlayerConst::getSwimHeadSlidingGravity() const {
    return mSwimHeadSlidingGravity;
}
int PlayerConst::getSwimHeadSlidingEndBrakeFrame() const {
    return mSwimHeadSlidingEndBrakeFrame;
}
int PlayerConst::getSwimHeadSlidingEndSpeedMin() const {
    return mSwimHeadSlidingEndSpeedMin;
}
float PlayerConst::getSwimJumpHipDropSpeed() const {
    return mSwimJumpHipDropSpeed;
}
float PlayerConst::getSwimJumpHipDropBrakeV() const {
    return mSwimJumpHipDropBrakeV;
}
float PlayerConst::getSwimJumpHipDropBrakeVCeiling() const {
    return mSwimJumpHipDropBrakeVCeiling;
}
float PlayerConst::getSwimJumpHipDropGravity() const {
    return mSwimJumpHipDropGravity;
}
float PlayerConst::getSwimJumpHipDropCancelSpeed() const {
    return mSwimJumpHipDropCancelSpeed;
}
float PlayerConst::getSwimJumpHipDropAccelH() const {
    return mSwimJumpHipDropAccelH;
}
float PlayerConst::getSwimJumpHipDropMoveSpeedH() const {
    return mSwimJumpHipDropMoveSpeedH;
}
float PlayerConst::getSwimJumpHipDropPopSpeed() const {
    return mSwimJumpHipDropPopSpeed;
}
float PlayerConst::getSwimJumpHipDropPopJumpAdd() const {
    return mSwimJumpHipDropPopJumpAdd;
}
float PlayerConst::getSwimTramplePower() const {
    return mSwimTramplePower;
}
float PlayerConst::getDiveTramplePower() const {
    return mDiveTramplePower;
}
int PlayerConst::getDiveTrampleCancelFrame() const {
    return mDiveTrampleCancelFrame;
}
float PlayerConst::getDamageSwimPushPower() const {
    return mDamageSwimPushPower;
}
float PlayerConst::getDamageSwimGravity() const {
    return mDamageSwimGravity;
}
int PlayerConst::getDamageSwimCancelFrame() const {
    return mDamageSwimCancelFrame;
}
float PlayerConst::getDamageSwimSurfaceGravity() const {
    return mDamageSwimSurfaceGravity;
}
float PlayerConst::getDamageSwimSurfaceHopPower() const {
    return mDamageSwimSurfaceHopPower;
}
float PlayerConst::getDamageSwimSurfacePushPower() const {
    return mDamageSwimSurfacePushPower;
}
float PlayerConst::getDamageSwimSurfaceLandSpeed() const {
    return mDamageSwimSurfaceLandSpeed;
}
float PlayerConst::getDamageSwimSurfaceLandBrake() const {
    return mDamageSwimSurfaceLandBrake;
}
float PlayerConst::getDamageSwimSurfaceLandEndSpeed() const {
    return mDamageSwimSurfaceLandEndSpeed;
}
int PlayerConst::getDamageSwimSurfaceCancelFrame() const {
    return mDamageSwimSurfaceCancelFrame;
}
float PlayerConst::getDamageSwimBrakeRateGround() const {
    return mDamageSwimBrakeRateGround;
}
int PlayerConst::getOxygenReduceFrame() const {
    return mOxygenReduceFrame;
}
int PlayerConst::getOxygenNoReduceFrame() const {
    return mOxygenNoReduceFrame;
}
int PlayerConst::getOxygenRecoveryFrame() const {
    return mOxygenRecoveryFrame;
}
int PlayerConst::getOxygenDamageInterval() const {
    return mOxygenDamageinterval;
}
int PlayerConst::getIceWaterDamageInterval() const {
    return mIceWaterDamageinterval;
}
int PlayerConst::getIceWaterRecoveryFrame() const {
    return mIceWaterRecoveryFrame;
}
float PlayerConst::getMoveAnimSpeedMax() const {
    return mMoveAnimSpeedMax;
}
float PlayerConst::getAnimFrameRateSpeedMin() const {
    return mAnimFrameRateSpeedMin;
}
float PlayerConst::getRunBorderSpeed() const {
    return mRunBorderSpeed;
}
float PlayerConst::getRunBlendRange() const {
    return mRunBlendRange;
}
float PlayerConst::getDashBorderSpeed() const {
    return mDashBorderSpeed;
}
float PlayerConst::getDashBlendRange() const {
    return mDashBlendRange;
}
float PlayerConst::getDashFastBorderSpeed() const {
    return mDashFastBorderSpeed;
}
float PlayerConst::getDashFastBlendRange() const {
    return mDashFastBlendRange;
}
float PlayerConst::getAnimFrameRateSpeedMax() const {
    return mAnimFrameRateSpeedMax;
}
float PlayerConst::getAnimFrameRateRunStart() const {
    return mAnimFrameRateRunStart;
}
float PlayerConst::getAnimFrameRateMinRun() const {
    return mAnimFrameRateMinRun;
}
float PlayerConst::getAnimFrameRateMaxRun() const {
    return mAnimFrameRateMaxRun;
}
float PlayerConst::getAnimFrameRateMaxDash() const {
    return mAnimFrameRateMaxDash;
}
float PlayerConst::getAnimFrameRateMaxDashFast() const {
    return mAnimFrameRateMaxDashFast;
}
float PlayerConst::getRunStartPlayFrameScale() const {
    return mRunStartPlayFrameScale;
}
int PlayerConst::getRunStartBlendFrame() const {
    return mRunStartBlendFrame;
}
float PlayerConst::getDamageFireRunAnimRate() const {
    return mDamageFireRunAnimRate;
}
float PlayerConst::getRunSkateAnimSpeedOffset() const {
    return mRunSkateAnimSpeedOffset;
}
float PlayerConst::getAnimFrameRateRange2D() const {
    return mAnimFrameRateRange2D;
}
float PlayerConst::getAnimFrameRateMinRun2D() const {
    return mAnimFrameRateMinRun2D;
}
float PlayerConst::getAnimFrameRateMaxRun2D() const {
    return mAnimFrameRateMaxRun2D;
}
int PlayerConst::getIKBlendFrameRun() const {
    return mIKBlendFrameRun;
}
float PlayerConst::getIKBlendRateRunMin() const {
    return mIKBlendRateRunMin;
}
float PlayerConst::getIKBlendRateRunMax() const {
    return mIKBlendRateRunMax;
}
float PlayerConst::getRollingAnimBorderSpeedMin() const {
    return mRollingAnimBorderSpeedMin;
}
float PlayerConst::getRollingAnimBorderSpeedMax() const {
    return mRollingAnimBorderSpeedMax;
}
float PlayerConst::getRollingAnimFrameRateMin() const {
    return mRollingAnimFrameRateMin;
}
float PlayerConst::getRollingAnimFrameRateMax() const {
    return mRollingAnimFrameRateMax;
}
int PlayerConst::getSwimPaddleAnimInterval() const {
    return mSwimPaddleAniminterval;
}
int PlayerConst::getSwimPaddleAnimRateIntervalMax() const {
    return mSwimPaddleAnimRateintervalMax;
}
int PlayerConst::getSwimPaddleAnimRateIntervalMin() const {
    return mSwimPaddleAnimRateintervalMin;
}
float PlayerConst::getSwimPaddleAnimMaxRate() const {
    return mSwimPaddleAnimMaxRate;
}
float PlayerConst::getSwimBentForwardMax() const {
    return mSwimBentForwardMax;
}
float PlayerConst::getSwimBentForwardBlendRate() const {
    return mSwimBentForwardBlendRate;
}
float PlayerConst::getSwimBentSideMax() const {
    return mSwimBentSideMax;
}
float PlayerConst::getSwimBentSpineMax() const {
    return mSwimBentSpineMax;
}
float PlayerConst::getSwimBentSideBlendRate() const {
    return mSwimBentSideBlendRate;
}
float PlayerConst::getSwimBentFrontMax() const {
    return mSwimBentFrontMax;
}
float PlayerConst::getSwimBentFrontBlendRate() const {
    return mSwimBentFrontBlendRate;
}
float PlayerConst::getSwimWalkAnimSpeedMax() const {
    return mSwimWalkAnimSpeedMax;
}
float PlayerConst::getSwimWalkAnimSpeedMin() const {
    return mSwimWalkAnimSpeedMin;
}
float PlayerConst::getSwimWalkAnimFrameRateMax() const {
    return mSwimWalkAnimFrameRateMax;
}
float PlayerConst::getSwimWalkAnimFrameRateMin() const {
    return mSwimWalkAnimFrameRateMin;
}
float PlayerConst::getSandSinkBorderMin() const {
    return mSandSinkBorderMin;
}
float PlayerConst::getSandSinkBorderMax() const {
    return mSandSinkBorderMax;
}
float PlayerConst::getSandSinkBorderRateMin() const {
    return mSandSinkBorderRateMin;
}
float PlayerConst::getSandSinkBorderRateMax() const {
    return mSandSinkBorderRateMax;
}
float PlayerConst::getSandSinkFrameRateMin() const {
    return mSandSinkFrameRateMin;
}
float PlayerConst::getSandSinkFrameRateMax() const {
    return mSandSinkFrameRateMax;
}
int PlayerConst::getLookAtEyeKeepFrame() const {
    return mLookAtEyeKeepFrame;
}
int PlayerConst::getLookAtEyeKeepFrameInSight() const {
    return mLookAtEyeKeepFrameInSight;
}
int PlayerConst::getLookAtEyeKeepFrameWait() const {
    return mLookAtEyeKeepFrameWait;
}
float PlayerConst::getLookAtEyeDistance() const {
    return mLookAtEyeDistance;
}
float PlayerConst::getLookAtEyeAngleMinH() const {
    return mLookAtEyeAngleMinH;
}
float PlayerConst::getLookAtEyeAngleMinInSightH() const {
    return mLookAtEyeAngleMinInSightH;
}
float PlayerConst::getLookAtEyeAngleMaxH() const {
    return mLookAtEyeAngleMaxH;
}
float PlayerConst::getLookAtEyeAngleMinV() const {
    return mLookAtEyeAngleMinV;
}
float PlayerConst::getLookAtEyeAngleMinInSightV() const {
    return mLookAtEyeAngleMinInSightV;
}
float PlayerConst::getLookAtEyeAngleMaxV() const {
    return mLookAtEyeAngleMaxV;
}
float PlayerConst::getTiltEyeBorderStart() const {
    return mTiltEyeBorderStart;
}
float PlayerConst::getTiltEyeBorderEnd() const {
    return mTiltEyeBorderEnd;
}
float PlayerConst::getTiltEyeAngleScale() const {
    return mTiltEyeAngleScale;
}
float PlayerConst::getCenterTiltRateMax() const {
    return mCenterTiltRateMax;
}
float PlayerConst::getNoseChildLocalOffset() const {
    return mNoseChildLocalOffset;
}
float PlayerConst::getNoseStability() const {
    return mNoseStability;
}
float PlayerConst::getNoseFriction() const {
    return mNoseFriction;
}
float PlayerConst::getNoseLimitDegree() const {
    return mNoseLimitDegree;
}
float PlayerConst::getMustacheChildLocalOffset() const {
    return mMustacheChildLocalOffset;
}
float PlayerConst::getMustacheStability() const {
    return mMustacheStability;
}
float PlayerConst::getMustacheFriction() const {
    return mMustacheFriction;
}
float PlayerConst::getMustacheLimitDegree() const {
    return mMustacheLimitDegree;
}
int PlayerConst::getCapInterpolateFrame() const {
    return mCapinterpolateFrame;
}
float PlayerConst::getCapChildLocalOffset() const {
    return mCapChildLocalOffset;
}
float PlayerConst::getCapStability() const {
    return mCapStability;
}
float PlayerConst::getCapFriction() const {
    return mCapFriction;
}
float PlayerConst::getCapLimitDegree() const {
    return mCapLimitDegree;
}
float PlayerConst::getCapTransStability() const {
    return mCapTransStability;
}
float PlayerConst::getCapTransFriction() const {
    return mCapTransFriction;
}
float PlayerConst::getCapTransLimit() const {
    return mCapTransLimit;
}
int PlayerConst::getCapManHeroEyesWaitAppearFrame() const {
    return mCapManHeroEyesWaitAppearFrame;
}
int PlayerConst::getDeadWipeStartDamage() const {
    return mDeadWipeStartDamage;
}
int PlayerConst::getDeadWipeWaitDamage() const {
    return mDeadWipeWaitDamage;
}
int PlayerConst::getDeadWipeStartAbyss() const {
    return mDeadWipeStartAbyss;
}
int PlayerConst::getDeadWipeWaitAbyss() const {
    return mDeadWipeWaitAbyss;
}
int PlayerConst::getDeadWipeStartAbyssWithCapMsg() const {
    return mDeadWipeStartAbyssWithCapMsg;
}
int PlayerConst::getDeadWipeWaitAbyssWithCapMsg() const {
    return mDeadWipeWaitAbyssWithCapMsg;
}
int PlayerConst::getDeadWipeStartPress() const {
    return mDeadWipeStartPress;
}
int PlayerConst::getDeadWipeWaitPress() const {
    return mDeadWipeWaitPress;
}
int PlayerConst::getDeadWipeStartSandSink() const {
    return mDeadWipeStartSandSink;
}
int PlayerConst::getDeadWipeWaitSandSink() const {
    return mDeadWipeWaitSandSink;
}
int PlayerConst::getDeadWipeStartNoOxygen() const {
    return mDeadWipeStartNoOxygen;
}
int PlayerConst::getDeadWipeWaitNoOxygen() const {
    return mDeadWipeWaitNoOxygen;
}
int PlayerConst::getDeadWipeStartIceWater() const {
    return mDeadWipeStartIceWater;
}
int PlayerConst::getDeadWipeWaitIceWater() const {
    return mDeadWipeWaitIceWater;
}
float PlayerConst::getCoinDashSpeed() const {
    return mCoinDashSpeed;
}
float PlayerConst::getCoinDashSpeedLimit() const {
    return mCoinDashSpeedLimit;
}
float PlayerConst::getAdditionalSpeedLimit() const {
    return mAdditionalSpeedLimit;
}
