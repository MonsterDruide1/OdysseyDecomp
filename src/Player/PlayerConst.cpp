#include "Player/PlayerConst.h"

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

f32 PlayerConst::getGravity() const {
    return mGravity;
}
f32 PlayerConst::getFrictionAttack() const {
    return mFrictionAttack;
}
f32 PlayerConst::getPushPower() const {
    return mPushPower;
}
f32 PlayerConst::getWaitPoseDegreeMax() const {
    return mWaitPoseDegreeMax;
}
f32 PlayerConst::getHillPoseDegreeMax() const {
    return mHillPoseDegreeMax;
}
f32 PlayerConst::getTiltPoseDegreeMax() const {
    return mTiltPoseDegreeMax;
}
f32 PlayerConst::getSlerpQuatRate() const {
    return mSlerpQuatRate;
}
f32 PlayerConst::getSlerpQuatRateWait() const {
    return mSlerpQuatRateWait;
}
f32 PlayerConst::getSlerpQuatGrav() const {
    return mSlerpQuatGrav;
}
s32 PlayerConst::getPreInputFrameCapThrow() const {
    return mPreInputFrameCapThrow;
}
s32 PlayerConst::getEnableActionFrameCapCatch() const {
    return mEnableActionFrameCapCatch;
}
f32 PlayerConst::getJumpPowerCapCatch() const {
    return mJumpPowerCapCatch;
}
f32 PlayerConst::getJumpGravityCapCatch() const {
    return mJumpGravityCapCatch;
}
s32 PlayerConst::getRunTimeContinuousThrow() const {
    return mRunTimeContinuousThrow;
}
f32 PlayerConst::getRunSpeedMaxContinuousThrow() const {
    return mRunSpeedMaxContinuousThrow;
}
s32 PlayerConst::getRunAccelFrameContinuousThrow() const {
    return mRunAccelFrameContinuousThrow;
}
f32 PlayerConst::getSeparateCheckHeight() const {
    return mSeparateCheckHeight;
}
f32 PlayerConst::getSeparateOffsetLerpRate() const {
    return mSeparateOffsetLerpRate;
}
f32 PlayerConst::getSeparateEnableThrowHeight() const {
    return mSeparateEnableThrowHeight;
}
f32 PlayerConst::getTall() const {
    return mTall;
}
f32 PlayerConst::getCollisionRadius() const {
    return mCollisionRadius;
}
f32 PlayerConst::getCollisionRadiusSquat() const {
    return mCollisionRadiusSquat;
}
f32 PlayerConst::getCollisionRadiusStand() const {
    return mCollisionRadiusStand;
}
f32 PlayerConst::getCollisionSmallStepHeight() const {
    return mCollisionSmallStepHeight;
}
f32 PlayerConst::getCollisionResetLimit() const {
    return mCollisionResetLimit;
}
f32 PlayerConst::getReflectCeilingPower() const {
    return mReflectCeilingPower;
}
f32 PlayerConst::getReflectTossPower() const {
    return mReflectTossPower;
}
f32 PlayerConst::getReflectUpperPunchScaleH() const {
    return mReflectUpperPunchScaleH;
}
f32 PlayerConst::getCollisionHitDownAngleH() const {
    return mCollisionHitDownAngleH;
}
f32 PlayerConst::getCollisionHitDownEscapeAngleV() const {
    return mCollisionHitDownEscapeAngleV;
}
f32 PlayerConst::getShadowDropHeightScale() const {
    return mShadowDropHeightScale;
}
f32 PlayerConst::getShadowDropNormalAdd() const {
    return mShadowDropNormalAdd;
}
f32 PlayerConst::getShadowDropLengthMin() const {
    return mShadowDropLengthMin;
}
f32 PlayerConst::getShadowDropLengthMax() const {
    return mShadowDropLengthMax;
}
f32 PlayerConst::getShadowDropLengthExtend() const {
    return mShadowDropLengthExtend;
}
f32 PlayerConst::getGravityDamage() const {
    return mGravityDamage;
}
f32 PlayerConst::getHopPowerDamage() const {
    return mHopPowerDamage;
}
f32 PlayerConst::getPushPowerDamage() const {
    return mPushPowerDamage;
}
s32 PlayerConst::getDamageCancelFrame() const {
    return mDamageCancelFrame;
}
s32 PlayerConst::getDamageInvalidCount() const {
    return mDamageInvalidCount;
}
s32 PlayerConst::getDamageInvalidCountRecovery() const {
    return mDamageInvalidCountRecovery;
}
s32 PlayerConst::getDamageInvalidCountAbyss() const {
    return mDamageInvalidCountAbyss;
}
f32 PlayerConst::getNormalMinSpeed2D() const {
    return mNormalMinSpeed2D;
}
f32 PlayerConst::getNormalMaxSpeed2D() const {
    return mNormalMaxSpeed2D;
}
f32 PlayerConst::getDashMaxSpeed2D() const {
    return mDashMaxSpeed2D;
}
s32 PlayerConst::getNormalAccelFrame2D() const {
    return mNormalAccelFrame2D;
}
s32 PlayerConst::getDashAccelFrame2D() const {
    return mDashAccelFrame2D;
}
s32 PlayerConst::getNormalDashAnimFrame2D() const {
    return mNormalDashAnimFrame2D;
}
s32 PlayerConst::getNormalBrakeFrame2D() const {
    return mNormalBrakeFrame2D;
}
s32 PlayerConst::getStickOnBrakeFrame2D() const {
    return mStickOnBrakeFrame2D;
}
s32 PlayerConst::getBrakeTurnStartFrame2D() const {
    return mBrakeTurnStartFrame2D;
}
f32 PlayerConst::getTurnEndSpeedRate2D() const {
    return mTurnEndSpeedRate2D;
}
f32 PlayerConst::getJumpPowerMin2DArea() const {
    return mJumpPowerMin2DArea;
}
f32 PlayerConst::getJumpPowerMax2DArea() const {
    return mJumpPowerMax2DArea;
}
f32 PlayerConst::getJumpPowerMinBorder2D() const {
    return mJumpPowerMinBorder2D;
}
f32 PlayerConst::getJumpPowerMaxBorder2D() const {
    return mJumpPowerMaxBorder2D;
}
f32 PlayerConst::getGravityMove() const {
    return mGravityMove;
}
f32 PlayerConst::getNormalMaxSpeed() const {
    return mNormalMaxSpeed;
}
f32 PlayerConst::getNormalMinSpeed() const {
    return mNormalMinSpeed;
}
s32 PlayerConst::getNormalAccelFrame() const {
    return mNormalAccelFrame;
}
f32 PlayerConst::getRunAccelAverageScale() const {
    return mRunAccelAverageScale;
}
s32 PlayerConst::getNormalBrakeFrame() const {
    return mNormalBrakeFrame;
}
f32 PlayerConst::getDashJudgeSpeed() const {
    return mDashJudgeSpeed;
}
s32 PlayerConst::getStickOnBrakeFrame() const {
    return mStickOnBrakeFrame;
}
s32 PlayerConst::getNormalDashAnimFrame() const {
    return mNormalDashAnimFrame;
}
f32 PlayerConst::getRunAfterTurnSpeedMax() const {
    return mRunAfterTurnSpeedMax;
}
f32 PlayerConst::getRunAfterTurnScale() const {
    return mRunAfterTurnScale;
}
s32 PlayerConst::getRunAfterTurnFrame() const {
    return mRunAfterTurnFrame;
}
s32 PlayerConst::getBrakeTurnStartFrame() const {
    return mBrakeTurnStartFrame;
}
f32 PlayerConst::getBrakeOnSpeedRate() const {
    return mBrakeOnSpeedRate;
}
s32 PlayerConst::getBrakeOnCounterBorder() const {
    return mBrakeOnCounterBorder;
}
s32 PlayerConst::getWallPushFrame() const {
    return mWallPushFrame;
}
s32 PlayerConst::getRunDeepDownFrame() const {
    return mRunDeepDownFrame;
}
s32 PlayerConst::getRunDeepDownMargine() const {
    return mRunDeepDownMargine;
}
s32 PlayerConst::getQuickTurnJumpFrame() const {
    return mQuickTurnJumpFrame;
}
s32 PlayerConst::getRoundAccelFrame() const {
    return mRoundAccelFrame;
}
s32 PlayerConst::getRoundBrakeFrame() const {
    return mRoundBrakeFrame;
}
f32 PlayerConst::getRoundFastDegree() const {
    return mRoundFastDegree;
}
s32 PlayerConst::getRoundAccelFrameFast() const {
    return mRoundAccelFrameFast;
}
f32 PlayerConst::getRoundMinDegree() const {
    return mRoundMinDegree;
}
s32 PlayerConst::getRoundBrakeFrameForce() const {
    return mRoundBrakeFrameForce;
}
f32 PlayerConst::getRoundFastDegreeForce() const {
    return mRoundFastDegreeForce;
}
f32 PlayerConst::getRoundLimitDegreeForce() const {
    return mRoundLimitDegreeForce;
}
f32 PlayerConst::getRoundLimitDegreeForceFast() const {
    return mRoundLimitDegreeForceFast;
}
s32 PlayerConst::getRoundAccelFrameForceFast() const {
    return mRoundAccelFrameForceFast;
}
f32 PlayerConst::getRoundLimitDegreeMin() const {
    return mRoundLimitDegreeMin;
}
f32 PlayerConst::getRoundLimitDegree() const {
    return mRoundLimitDegree;
}
s32 PlayerConst::getIceAccelFrame() const {
    return mIceAccelFrame;
}
s32 PlayerConst::getIceBrakeFrame() const {
    return mIceBrakeFrame;
}
s32 PlayerConst::getIceBrakeFrameHigh() const {
    return mIceBrakeFrameHigh;
}
s32 PlayerConst::getIceBrakeFrameWall() const {
    return mIceBrakeFrameWall;
}
s32 PlayerConst::getIceRoundAccelFrame() const {
    return mIceRoundAccelFrame;
}
s32 PlayerConst::getIceRoundAccelFrameFast() const {
    return mIceRoundAccelFrameFast;
}
s32 PlayerConst::getIceRoundBrakeFrame() const {
    return mIceRoundBrakeFrame;
}
f32 PlayerConst::getIceRoundFastDegree() const {
    return mIceRoundFastDegree;
}
f32 PlayerConst::getIceRoundMinDegree() const {
    return mIceRoundMinDegree;
}
f32 PlayerConst::getIceRoundLimitDegree() const {
    return mIceRoundLimitDegree;
}
f32 PlayerConst::getHillAddSpeed() const {
    return mHillAddSpeed;
}
f32 PlayerConst::getHillSubSpeed() const {
    return mHillSubSpeed;
}
s32 PlayerConst::getHillAccelAddFrame() const {
    return mHillAccelAddFrame;
}
s32 PlayerConst::getHillAccelSubFrame() const {
    return mHillAccelSubFrame;
}
f32 PlayerConst::getHillAccelSubAngleMin() const {
    return mHillAccelSubAngleMin;
}
f32 PlayerConst::getHillAccelSubAngleMax() const {
    return mHillAccelSubAngleMax;
}
f32 PlayerConst::getStandAngleMin() const {
    return mStandAngleMin;
}
f32 PlayerConst::getStandAngleMax() const {
    return mStandAngleMax;
}
f32 PlayerConst::getHillAngleSpeedMin() const {
    return mHillAngleSpeedMin;
}
f32 PlayerConst::getHillAngleSpeedMax() const {
    return mHillAngleSpeedMax;
}
s32 PlayerConst::getSpinCapThrowFrame() const {
    return mSpinCapThrowFrame;
}
s32 PlayerConst::getSpinCapThrowFrameAir() const {
    return mSpinCapThrowFrameAir;
}
s32 PlayerConst::getSpinCapThrowFrameSwim() const {
    return mSpinCapThrowFrameSwim;
}
s32 PlayerConst::getSpinCapThrowFrameSwing() const {
    return mSpinCapThrowFrameSwing;
}
s32 PlayerConst::getSpinCapThrowFrameContinuous() const {
    return mSpinCapThrowFrameContinuous;
}
s32 PlayerConst::getSpinAttackFrame() const {
    return mSpinAttackFrame;
}
s32 PlayerConst::getSpinBrakeFrame() const {
    return mSpinBrakeFrame;
}
f32 PlayerConst::getSpinAirJumpPower() const {
    return mSpinAirJumpPower;
}
f32 PlayerConst::getSpinAirSpeedMax() const {
    return mSpinAirSpeedMax;
}
f32 PlayerConst::getGravitySpinAir() const {
    return mGravitySpinAir;
}
f32 PlayerConst::getSlerpQuatRateSpinAir() const {
    return mSlerpQuatRateSpinAir;
}
f32 PlayerConst::getSpinBrakeRate() const {
    return mSpinBrakeRate;
}
f32 PlayerConst::getSpinBrakeSideAccel() const {
    return mSpinBrakeSideAccel;
}
f32 PlayerConst::getSpinBrakeSideBrakeRate() const {
    return mSpinBrakeSideBrakeRate;
}
f32 PlayerConst::getSpinBrakeSideMaxSpeedRate() const {
    return mSpinBrakeSideMaxSpeedRate;
}
f32 PlayerConst::getSpinRoundLimitDegree() const {
    return mSpinRoundLimitDegree;
}
f32 PlayerConst::getDamageFireJumpPower1st() const {
    return mDamageFireJumpPower1st;
}
f32 PlayerConst::getDamageFireJumpPower2nd() const {
    return mDamageFireJumpPower2nd;
}
f32 PlayerConst::getDamageFireJumpMoveSpeed() const {
    return mDamageFireJumpMoveSpeed;
}
f32 PlayerConst::getDamageFireCeilHitSpeed() const {
    return mDamageFireCeilHitSpeed;
}
f32 PlayerConst::getDamageFireGravity() const {
    return mDamageFireGravity;
}
s32 PlayerConst::getDamageFireNoGravityFrame() const {
    return mDamageFireNoGravityFrame;
}
s32 PlayerConst::getDamageFireRunTime() const {
    return mDamageFireRunTime;
}
f32 PlayerConst::getDamageFireRunSpeed() const {
    return mDamageFireRunSpeed;
}
f32 PlayerConst::getDamageFireRunBrakeFrame() const {
    return mDamageFireRunBrakeFrame;
}
s32 PlayerConst::getSandSinkDeadTime() const {
    return mSandSinkDeadTime;
}
s32 PlayerConst::getSandSinkBrakeHeightH() const {
    return mSandSinkBrakeHeightH;
}
s32 PlayerConst::getSandSinkBrakeHeightV() const {
    return mSandSinkBrakeHeightV;
}
f32 PlayerConst::getSandSinkHeight() const {
    return mSandSinkHeight;
}
f32 PlayerConst::getSandSinkCapThrow() const {
    return mSandSinkCapThrow;
}
f32 PlayerConst::getSandSinkBrakeMinH() const {
    return mSandSinkBrakeMinH;
}
f32 PlayerConst::getSandSinkBrakeMaxH() const {
    return mSandSinkBrakeMaxH;
}
f32 PlayerConst::getSandSinkBrakeMinV() const {
    return mSandSinkBrakeMinV;
}
f32 PlayerConst::getSandSinkBrakeMaxV() const {
    return mSandSinkBrakeMaxV;
}
f32 PlayerConst::getSlopeSlideAngleStart() const {
    return mSlopeSlideAngleStart;
}
f32 PlayerConst::getSlopeSlideAngleEnd() const {
    return mSlopeSlideAngleEnd;
}
f32 PlayerConst::getSlopeSlideAccel() const {
    return mSlopeSlideAccel;
}
f32 PlayerConst::getSlopeSlideBrake() const {
    return mSlopeSlideBrake;
}
f32 PlayerConst::getSlopeSlideMaxSpeed() const {
    return mSlopeSlideMaxSpeed;
}
f32 PlayerConst::getSlopeSlideSpeedEnd() const {
    return mSlopeSlideSpeedEnd;
}
f32 PlayerConst::getSlopeSlideSideAccel() const {
    return mSlopeSlideSideAccel;
}
f32 PlayerConst::getSlopeSlideSideBrake() const {
    return mSlopeSlideSideBrake;
}
f32 PlayerConst::getSlopeSlideSideMaxSpeed() const {
    return mSlopeSlideSideMaxSpeed;
}
f32 PlayerConst::getSlopeTurnDegree() const {
    return mSlopeTurnDegree;
}
s32 PlayerConst::getSlideInvalidFrame() const {
    return mSlideInvalidFrame;
}
s32 PlayerConst::getSlopeForceFrame() const {
    return mSlopeForceFrame;
}
f32 PlayerConst::getSlopeSlideForceSideAccel() const {
    return mSlopeSlideForceSideAccel;
}
f32 PlayerConst::getSlopeSlideForceSideBrake() const {
    return mSlopeSlideForceSideBrake;
}
f32 PlayerConst::getSlopeSlideForceSideMaxSpeed() const {
    return mSlopeSlideForceSideMaxSpeed;
}
f32 PlayerConst::getSlopeSlideForceTurnDegree() const {
    return mSlopeSlideForceTurnDegree;
}
f32 PlayerConst::getSlopeRollingSpeedStart() const {
    return mSlopeRollingSpeedStart;
}
f32 PlayerConst::getSlopeRollingSpeedBoost() const {
    return mSlopeRollingSpeedBoost;
}
f32 PlayerConst::getSlopeRollingMaxSpeed() const {
    return mSlopeRollingMaxSpeed;
}
s32 PlayerConst::getSlopeRollingFrameMinBoost() const {
    return mSlopeRollingFrameMinBoost;
}
s32 PlayerConst::getSlopeRollingFrameMin() const {
    return mSlopeRollingFrameMin;
}
f32 PlayerConst::getSlopeRollingStartJumpPower() const {
    return mSlopeRollingStartJumpPower;
}
f32 PlayerConst::getSlopeRollingStartSlideSpeed() const {
    return mSlopeRollingStartSlideSpeed;
}
f32 PlayerConst::getSlopeRollingAccel() const {
    return mSlopeRollingAccel;
}
f32 PlayerConst::getSlopeRollingBrake() const {
    return mSlopeRollingBrake;
}
f32 PlayerConst::getSlopeRollingAgainst() const {
    return mSlopeRollingAgainst;
}
f32 PlayerConst::getSlopeRollingAnglePowerMax() const {
    return mSlopeRollingAnglePowerMax;
}
f32 PlayerConst::getSlopeRollingSpeedEnd() const {
    return mSlopeRollingSpeedEnd;
}
f32 PlayerConst::getSlopeRollingSideAccel() const {
    return mSlopeRollingSideAccel;
}
f32 PlayerConst::getSlopeRollingSideBrake() const {
    return mSlopeRollingSideBrake;
}
f32 PlayerConst::getSlopeRollingSideMaxSpeed() const {
    return mSlopeRollingSideMaxSpeed;
}
s32 PlayerConst::getSlopeRollingUnRollFrame() const {
    return mSlopeRollingUnRollFrame;
}
f32 PlayerConst::getSlopeRollingEndBrake() const {
    return mSlopeRollingEndBrake;
}
f32 PlayerConst::getSlopeRollingEndBrakeEndSpeed() const {
    return mSlopeRollingEndBrakeEndSpeed;
}
f32 PlayerConst::getSlopeRollingReStartAccel() const {
    return mSlopeRollingReStartAccel;
}
f32 PlayerConst::getSlopeRollingReStartMaxAdd() const {
    return mSlopeRollingReStartMaxAdd;
}
s32 PlayerConst::getSlopeRollingReStartInterval() const {
    return mSlopeRollingReStartinterval;
}
s32 PlayerConst::getSlopeRollingReStartSwing() const {
    return mSlopeRollingReStartSwing;
}
s32 PlayerConst::getSlopeRollingReStartCharge() const {
    return mSlopeRollingReStartCharge;
}
s32 PlayerConst::getSlopeRollingReStartForce() const {
    return mSlopeRollingReStartForce;
}
f32 PlayerConst::getSlopeRollingAccelOnSkate() const {
    return mSlopeRollingAccelOnSkate;
}
f32 PlayerConst::getSlopeRollingSideAccelOnSkate() const {
    return mSlopeRollingSideAccelOnSkate;
}
f32 PlayerConst::getSlopeRollingBrakeOnSkate() const {
    return mSlopeRollingBrakeOnSkate;
}
s32 PlayerConst::getExtendFrame() const {
    return mExtendFrame;
}
f32 PlayerConst::getJumpInertiaRate() const {
    return mJumpInertiaRate;
}
f32 PlayerConst::getJumpPowerMin() const {
    return mJumpPowerMin;
}
f32 PlayerConst::getJumpPowerMax() const {
    return mJumpPowerMax;
}
f32 PlayerConst::getJumpGravity() const {
    return mJumpGravity;
}
f32 PlayerConst::getJumpBaseSpeedMax() const {
    return mJumpBaseSpeedMax;
}
f32 PlayerConst::getJumpMoveSpeedMin() const {
    return mJumpMoveSpeedMin;
}
f32 PlayerConst::getJumpMoveSpeedMax() const {
    return mJumpMoveSpeedMax;
}
f32 PlayerConst::getJumpAccelFront() const {
    return mJumpAccelFront;
}
f32 PlayerConst::getJumpAccelBack() const {
    return mJumpAccelBack;
}
f32 PlayerConst::getJumpAccelTurn() const {
    return mJumpAccelTurn;
}
f32 PlayerConst::getJumpTurnAngleStart() const {
    return mJumpTurnAngleStart;
}
f32 PlayerConst::getJumpTurnAngleLimit() const {
    return mJumpTurnAngleLimit;
}
f32 PlayerConst::getJumpTurnAngleFast() const {
    return mJumpTurnAngleFast;
}
f32 PlayerConst::getJumpTurnAngleFastLimit() const {
    return mJumpTurnAngleFastLimit;
}
s32 PlayerConst::getJumpTurnAccelFrame() const {
    return mJumpTurnAccelFrame;
}
s32 PlayerConst::getJumpTurnAccelFrameFast() const {
    return mJumpTurnAccelFrameFast;
}
s32 PlayerConst::getJumpTurnBrakeFrame() const {
    return mJumpTurnBrakeFrame;
}
f32 PlayerConst::getTrampleGravity() const {
    return mTrampleGravity;
}
f32 PlayerConst::getTrampleJumpPower() const {
    return mTrampleJumpPower;
}
f32 PlayerConst::getTrampleHighGravity() const {
    return mTrampleHighGravity;
}
f32 PlayerConst::getTrampleHighJumpPower() const {
    return mTrampleHighJumpPower;
}
f32 PlayerConst::getTrampleGravity2D() const {
    return mTrampleGravity2D;
}
f32 PlayerConst::getTrampleJumpPower2D() const {
    return mTrampleJumpPower2D;
}
f32 PlayerConst::getTrampleHighGravity2D() const {
    return mTrampleHighGravity2D;
}
f32 PlayerConst::getTrampleHighJumpPower2D() const {
    return mTrampleHighJumpPower2D;
}
f32 PlayerConst::getTrampleHipDropGravity() const {
    return mTrampleHipDropGravity;
}
f32 PlayerConst::getTrampleHipDropJumpPower() const {
    return mTrampleHipDropJumpPower;
}
f32 PlayerConst::getTrampleRisingBrakeVelH() const {
    return mTrampleRisingBrakeVelH;
}
f32 PlayerConst::getTrampleJumpCodePower() const {
    return mTrampleJumpCodePower;
}
f32 PlayerConst::getTrampleJumpCodePowerSmall() const {
    return mTrampleJumpCodePowerSmall;
}
f32 PlayerConst::getCapLeapFrogJumpGravity() const {
    return mCapLeapFrogJumpGravity;
}
f32 PlayerConst::getCapLeapFrogJumpPower() const {
    return mCapLeapFrogJumpPower;
}
f32 PlayerConst::getCapLeapFrogJumpPowerAir() const {
    return mCapLeapFrogJumpPowerAir;
}
f32 PlayerConst::getObjLeapFrogJumpPower() const {
    return mObjLeapFrogJumpPower;
}
f32 PlayerConst::getObjLeapFrogJumpPowerHigh() const {
    return mObjLeapFrogJumpPowerHigh;
}
f32 PlayerConst::getCapHeadSpringJumpGravity() const {
    return mCapHeadSpringJumpGravity;
}
f32 PlayerConst::getCapHeadSpringJumpPower() const {
    return mCapHeadSpringJumpPower;
}
f32 PlayerConst::getCapHeadSpringJumpGravityHigh() const {
    return mCapHeadSpringJumpGravityHigh;
}
f32 PlayerConst::getCapHeadSpringJumpPowerHigh() const {
    return mCapHeadSpringJumpPowerHigh;
}
f32 PlayerConst::getCapHeadSpringSpeedMax() const {
    return mCapHeadSpringSpeedMax;
}
s32 PlayerConst::getContinuousJumpPreInputFrame() const {
    return mContinuousJumpPreInputFrame;
}
s32 PlayerConst::getContinuousJumpCount() const {
    return mContinuousJumpCount;
}
s32 PlayerConst::getContinuousJumpTimer() const {
    return mContinuousJumpTimer;
}
f32 PlayerConst::getContinuousJumpPowerMin() const {
    return mContinuousJumpPowerMin;
}
f32 PlayerConst::getJumpPowerMax2nd() const {
    return mJumpPowerMax2nd;
}
f32 PlayerConst::getJumpGravity2nd() const {
    return mJumpGravity2nd;
}
f32 PlayerConst::getJumpPowerMax3rd() const {
    return mJumpPowerMax3rd;
}
f32 PlayerConst::getJumpGravity3rd() const {
    return mJumpGravity3rd;
}
f32 PlayerConst::getSpinFlowerJumpGravity() const {
    return mSpinFlowerJumpGravity;
}
f32 PlayerConst::getSpinFlowerJumpFallSpeedMax() const {
    return mSpinFlowerJumpFallSpeedMax;
}
f32 PlayerConst::getSpinFlowerJumpMovePower() const {
    return mSpinFlowerJumpMovePower;
}
f32 PlayerConst::getSpinFlowerJumpVelMax() const {
    return mSpinFlowerJumpVelMax;
}
s32 PlayerConst::getSpinFlowerJumpStayFrame() const {
    return mSpinFlowerJumpStayFrame;
}
f32 PlayerConst::getSpinFlowerJumpStaySpeedMax() const {
    return mSpinFlowerJumpStaySpeedMax;
}
f32 PlayerConst::getSpinFlowerJumpNoInputBrake() const {
    return mSpinFlowerJumpNoInputBrake;
}
f32 PlayerConst::getSpinFlowerJumpDownFallInitSpeed() const {
    return mSpinFlowerJumpDownFallInitSpeed;
}
f32 PlayerConst::getSpinFlowerJumpDownFallPower() const {
    return mSpinFlowerJumpDownFallPower;
}
f32 PlayerConst::getSpinFlowerJumpDownFallSpeedMax() const {
    return mSpinFlowerJumpDownFallSpeedMax;
}
f32 PlayerConst::getJumpGravityForceRun() const {
    return mJumpGravityForceRun;
}
f32 PlayerConst::getJumpPowerForceRun() const {
    return mJumpPowerForceRun;
}
f32 PlayerConst::getCapCatchPopPower() const {
    return mCapCatchPopPower;
}
f32 PlayerConst::getCapCatchPopGravity() const {
    return mCapCatchPopGravity;
}
f32 PlayerConst::getSquatJumpGravity() const {
    return mSquatJumpGravity;
}
f32 PlayerConst::getSquatJumpPower() const {
    return mSquatJumpPower;
}
f32 PlayerConst::getSquatJumpBackPower() const {
    return mSquatJumpBackPower;
}
f32 PlayerConst::getSquatJumpMovePowerFront() const {
    return mSquatJumpMovePowerFront;
}
f32 PlayerConst::getSquatJumpMovePowerSide() const {
    return mSquatJumpMovePowerSide;
}
f32 PlayerConst::getSquatJumpMoveSpeedMax() const {
    return mSquatJumpMoveSpeedMax;
}
f32 PlayerConst::getTurnJumpGravity() const {
    return mTurnJumpGravity;
}
f32 PlayerConst::getTurnJumpPower() const {
    return mTurnJumpPower;
}
f32 PlayerConst::getTurnJumpVelH() const {
    return mTurnJumpVelH;
}
f32 PlayerConst::getTurnJumpBrake() const {
    return mTurnJumpBrake;
}
f32 PlayerConst::getTurnJumpAccel() const {
    return mTurnJumpAccel;
}
f32 PlayerConst::getTurnJumpSideAccel() const {
    return mTurnJumpSideAccel;
}
f32 PlayerConst::getLongJumpAccel() const {
    return mLongJumpAccel;
}
f32 PlayerConst::getLongJumpBrake() const {
    return mLongJumpBrake;
}
f32 PlayerConst::getLongJumpSideAccel() const {
    return mLongJumpSideAccel;
}
f32 PlayerConst::getLongJumpGravity() const {
    return mLongJumpGravity;
}
f32 PlayerConst::getLongJumpJumpPow() const {
    return mLongJumpJumpPow;
}
f32 PlayerConst::getLongJumpMovePow() const {
    return mLongJumpMovePow;
}
f32 PlayerConst::getLongJumpInitSpeed() const {
    return mLongJumpInitSpeed;
}
f32 PlayerConst::getLongJumpSpeed() const {
    return mLongJumpSpeed;
}
f32 PlayerConst::getLongJumpSpeedMin() const {
    return mLongJumpSpeedMin;
}
s32 PlayerConst::getContinuousLongJumpCount() const {
    return mContinuousLongJumpCount;
}
s32 PlayerConst::getContinuousLongJumpTimer() const {
    return mContinuousLongJumpTimer;
}
f32 PlayerConst::getGravityAir() const {
    return mGravityAir;
}
f32 PlayerConst::getFrictionAir() const {
    return mFrictionAir;
}
f32 PlayerConst::getFallSpeedMax() const {
    return mFallSpeedMax;
}
f32 PlayerConst::getLongFallDistance() const {
    return mLongFallDistance;
}
f32 PlayerConst::getFallWallScaleVelocity() const {
    return mFallWallScaleVelocity;
}
s32 PlayerConst::getDownFallFrameMin() const {
    return mDownFallFrameMin;
}
f32 PlayerConst::getGravityWallSlide() const {
    return mGravityWallSlide;
}
f32 PlayerConst::getWallHeightLowLimit() const {
    return mWallHeightLowLimit;
}
f32 PlayerConst::getWallKeepDegree() const {
    return mWallKeepDegree;
}
s32 PlayerConst::getWallKeepFrame() const {
    return mWallKeepFrame;
}
f32 PlayerConst::getWallJumpGravity() const {
    return mWallJumpGravity;
}
f32 PlayerConst::getWallJumpHSpeed() const {
    return mWallJumpHSpeed;
}
f32 PlayerConst::getWallJumpPower() const {
    return mWallJumpPower;
}
s32 PlayerConst::getWallJumpInvalidateInputFrame() const {
    return mWallJumpInvalidateInputFrame;
}
s32 PlayerConst::getWallInhibitAfterPunch() const {
    return mWallInhibitAfterPunch;
}
f32 PlayerConst::getWallFollowAngleH() const {
    return mWallFollowAngleH;
}
f32 PlayerConst::getWallFollowAngleV() const {
    return mWallFollowAngleV;
}
f32 PlayerConst::getWallCatchDegree() const {
    return mWallCatchDegree;
}
f32 PlayerConst::getWallCatchHeightEdgeTop() const {
    return mWallCatchHeightEdgeTop;
}
f32 PlayerConst::getWallCatchHeightBottom() const {
    return mWallCatchHeightBottom;
}
f32 PlayerConst::getWallCatchKeepDegree() const {
    return mWallCatchKeepDegree;
}
f32 PlayerConst::getWallCatchMoveDegree() const {
    return mWallCatchMoveDegree;
}
f32 PlayerConst::getWallCatchMoveSpeed() const {
    return mWallCatchMoveSpeed;
}
f32 PlayerConst::getWallCatchMoveHeightRange() const {
    return mWallCatchMoveHeightRange;
}
s32 PlayerConst::getWallCatchMoveInterpolate() const {
    return mWallCatchMoveinterpolate;
}
s32 PlayerConst::getWallCatchMoveFrame() const {
    return mWallCatchMoveFrame;
}
s32 PlayerConst::getWallCatchMoveFrameFast() const {
    return mWallCatchMoveFrameFast;
}
s32 PlayerConst::getWallCatchMoveFrameSwing() const {
    return mWallCatchMoveFrameSwing;
}
f32 PlayerConst::getWallCatchInputRepeatAngle() const {
    return mWallCatchInputRepeatAngle;
}
f32 PlayerConst::getWallClimbDegree() const {
    return mWallClimbDegree;
}
s32 PlayerConst::getWallClimbJumpStartFrame() const {
    return mWallClimbJumpStartFrame;
}
s32 PlayerConst::getWallClimbJumpEndFrame() const {
    return mWallClimbJumpEndFrame;
}
s32 PlayerConst::getWallClimbStartFrame() const {
    return mWallClimbStartFrame;
}
f32 PlayerConst::getWallClimbGravity() const {
    return mWallClimbGravity;
}
f32 PlayerConst::getWallFallJumpSpeed() const {
    return mWallFallJumpSpeed;
}
f32 PlayerConst::getWallClimbJumpSpeedV() const {
    return mWallClimbJumpSpeedV;
}
f32 PlayerConst::getWallClimbJumpSpeedH() const {
    return mWallClimbJumpSpeedH;
}
f32 PlayerConst::getWallClimbJumpGravity() const {
    return mWallClimbJumpGravity;
}
s32 PlayerConst::getWallClimbJumpInvalidFrame() const {
    return mWallClimbJumpInvalidFrame;
}
f32 PlayerConst::getWallCatchHipLocalOffset() const {
    return mWallCatchHipLocalOffset;
}
f32 PlayerConst::getWallCatchHipStability() const {
    return mWallCatchHipStability;
}
f32 PlayerConst::getWallCatchHipFriction() const {
    return mWallCatchHipFriction;
}
f32 PlayerConst::getWallCatchHipLimitDegree() const {
    return mWallCatchHipLimitDegree;
}
f32 PlayerConst::getWallCatchStainAreaOffset() const {
    return mWallCatchStainAreaOffset;
}
f32 PlayerConst::getGrabCeilRange() const {
    return mGrabCeilRange;
}
f32 PlayerConst::getGrabCeilBodyRadius() const {
    return mGrabCeilBodyRadius;
}
f32 PlayerConst::getGrabCeilLeaveSpeedMin() const {
    return mGrabCeilLeaveSpeedMin;
}
f32 PlayerConst::getGrabCeilLeavePopPower() const {
    return mGrabCeilLeavePopPower;
}
f32 PlayerConst::getGrabCeilLeavePopGravity() const {
    return mGrabCeilLeavePopGravity;
}
f32 PlayerConst::getGrabCeilSwingStartOffset() const {
    return mGrabCeilSwingStartOffset;
}
f32 PlayerConst::getGrabCeilReverseInputBorder() const {
    return mGrabCeilReverseInputBorder;
}
f32 PlayerConst::getGrabCeilInputPowerBorder() const {
    return mGrabCeilInputPowerBorder;
}
f32 PlayerConst::getGrabCeilSwingWaitEnergy() const {
    return mGrabCeilSwingWaitEnergy;
}
f32 PlayerConst::getGrabCeilEnableJumpEnergy() const {
    return mGrabCeilEnableJumpEnergy;
}
f32 PlayerConst::getGrabCeilEnableJumpEnergyMax() const {
    return mGrabCeilEnableJumpEnergyMax;
}
f32 PlayerConst::getGrabCeilJumpForceAngle() const {
    return mGrabCeilJumpForceAngle;
}
f32 PlayerConst::getGrabCeilJumpPower() const {
    return mGrabCeilJumpPower;
}
f32 PlayerConst::getGrabCeilJumpMoveMin() const {
    return mGrabCeilJumpMoveMin;
}
f32 PlayerConst::getGrabCeilJumpMoveMax() const {
    return mGrabCeilJumpMoveMax;
}
f32 PlayerConst::getGrabCeilJumpGravity() const {
    return mGrabCeilJumpGravity;
}
s32 PlayerConst::getGrabCeilJumpInvalidFrame() const {
    return mGrabCeilJumpInvalidFrame;
}
s32 PlayerConst::getGrabCeilEnableNextFrame() const {
    return mGrabCeilEnableNextFrame;
}
s32 PlayerConst::getGrabCeilEnableFallSnapFrame() const {
    return mGrabCeilEnableFallSnapFrame;
}
s32 PlayerConst::getPoleClimbPreInputSwing() const {
    return mPoleClimbPreInputSwing;
}
f32 PlayerConst::getPoleClimbInputRepeatAngle() const {
    return mPoleClimbInputRepeatAngle;
}
f32 PlayerConst::getPoleClimbInputDegreeMove() const {
    return mPoleClimbInputDegreeMove;
}
f32 PlayerConst::getPoleClimbCatchRange() const {
    return mPoleClimbCatchRange;
}
f32 PlayerConst::getPoleClimbCatchRangeMin() const {
    return mPoleClimbCatchRangeMin;
}
f32 PlayerConst::getPoleClimbCatchRangeMax() const {
    return mPoleClimbCatchRangeMax;
}
f32 PlayerConst::getPoleClimbJointAngleMin() const {
    return mPoleClimbJointAngleMin;
}
f32 PlayerConst::getPoleClimbJointAngleMax() const {
    return mPoleClimbJointAngleMax;
}
f32 PlayerConst::getPoleClimbJointRangeMin() const {
    return mPoleClimbJointRangeMin;
}
f32 PlayerConst::getPoleClimbJointRangeMax() const {
    return mPoleClimbJointRangeMax;
}
f32 PlayerConst::getPoleClimbMoveWallDegree() const {
    return mPoleClimbMoveWallDegree;
}
f32 PlayerConst::getPoleClimbUpMargine() const {
    return mPoleClimbUpMargine;
}
f32 PlayerConst::getPoleClimbUpSpeed() const {
    return mPoleClimbUpSpeed;
}
s32 PlayerConst::getPoleClimbUpFrame() const {
    return mPoleClimbUpFrame;
}
s32 PlayerConst::getPoleClimbUpFrameFast() const {
    return mPoleClimbUpFrameFast;
}
s32 PlayerConst::getPoleClimbUpFrameSwing() const {
    return mPoleClimbUpFrameSwing;
}
f32 PlayerConst::getPoleClimbDownSpeed() const {
    return mPoleClimbDownSpeed;
}
f32 PlayerConst::getPoleClimbDownSpeedFast() const {
    return mPoleClimbDownSpeedFast;
}
f32 PlayerConst::getPoleClimbDownSpeedSwing() const {
    return mPoleClimbDownSpeedSwing;
}
s32 PlayerConst::getPoleClimbDownFrame() const {
    return mPoleClimbDownFrame;
}
s32 PlayerConst::getPoleClimbDownKeepTime() const {
    return mPoleClimbDownKeepTime;
}
f32 PlayerConst::getPoleClimbTurnDist() const {
    return mPoleClimbTurnDist;
}
s32 PlayerConst::getPoleClimbTurnFrame() const {
    return mPoleClimbTurnFrame;
}
s32 PlayerConst::getPoleClimbTurnStopFrame() const {
    return mPoleClimbTurnStopFrame;
}
s32 PlayerConst::getPoleTopStartFrame() const {
    return mPoleTopStartFrame;
}
s32 PlayerConst::getPoleTopEndFrame() const {
    return mPoleTopEndFrame;
}
f32 PlayerConst::getPoleTopTurnSpeed() const {
    return mPoleTopTurnSpeed;
}
f32 PlayerConst::getPoleTopEndUnderOffsetY() const {
    return mPoleTopEndUnderOffsetY;
}
s32 PlayerConst::getGroundSpinFrame() const {
    return mGroundSpinFrame;
}
f32 PlayerConst::getGroundSpinMoveSpeedMax() const {
    return mGroundSpinMoveSpeedMax;
}
f32 PlayerConst::getGroundSpinAccelRate() const {
    return mGroundSpinAccelRate;
}
f32 PlayerConst::getGroundSpinBrakeRate() const {
    return mGroundSpinBrakeRate;
}
f32 PlayerConst::getSpinJumpGravity() const {
    return mSpinJumpGravity;
}
f32 PlayerConst::getSpinJumpPower() const {
    return mSpinJumpPower;
}
f32 PlayerConst::getSpinJumpMoveSpeedMax() const {
    return mSpinJumpMoveSpeedMax;
}
f32 PlayerConst::getSpinJumpDownFallInitSpeed() const {
    return mSpinJumpDownFallInitSpeed;
}
f32 PlayerConst::getSpinJumpDownFallPower() const {
    return mSpinJumpDownFallPower;
}
f32 PlayerConst::getSpinJumpDownFallSpeedMax() const {
    return mSpinJumpDownFallSpeedMax;
}
f32 PlayerConst::getSquatBrakeEndSpeed() const {
    return mSquatBrakeEndSpeed;
}
f32 PlayerConst::getSquatAccelRate() const {
    return mSquatAccelRate;
}
f32 PlayerConst::getSquatBrakeRate() const {
    return mSquatBrakeRate;
}
f32 PlayerConst::getSquatBrakeRateOnSkate() const {
    return mSquatBrakeRateOnSkate;
}
f32 PlayerConst::getSquatBrakeSideAccel() const {
    return mSquatBrakeSideAccel;
}
f32 PlayerConst::getSquatBrakeSideRate() const {
    return mSquatBrakeSideRate;
}
f32 PlayerConst::getSquatBrakeSideAccelOnSkate() const {
    return mSquatBrakeSideAccelOnSkate;
}
f32 PlayerConst::getSquatBrakeSideRateOnSkate() const {
    return mSquatBrakeSideRateOnSkate;
}
f32 PlayerConst::getSquatBrakeSideMaxSpeedRate() const {
    return mSquatBrakeSideMaxSpeedRate;
}
f32 PlayerConst::getSquatWalkSpeed() const {
    return mSquatWalkSpeed;
}
f32 PlayerConst::getSquatWalkTurnSpeed() const {
    return mSquatWalkTurnSpeed;
}
s32 PlayerConst::getSquatWalkTurnFrame() const {
    return mSquatWalkTurnFrame;
}
f32 PlayerConst::getSquatJumpCeilSlideSpeed2D() const {
    return mSquatJumpCeilSlideSpeed2D;
}
f32 PlayerConst::getHipDropSpeed() const {
    return mHipDropSpeed;
}
f32 PlayerConst::getHipDropGravity() const {
    return mHipDropGravity;
}
f32 PlayerConst::getHipDropSpeedMax() const {
    return mHipDropSpeedMax;
}
s32 PlayerConst::getHipDropLandCancelFrame() const {
    return mHipDropLandCancelFrame;
}
f32 PlayerConst::getHipDropHeight() const {
    return mHipDropHeight;
}
s32 PlayerConst::getHipDropMsgInterval() const {
    return mHipDropMsginterval;
}
f32 PlayerConst::getJumpHipDropPower() const {
    return mJumpHipDropPower;
}
s32 PlayerConst::getJumpHipDropPermitBeginFrame() const {
    return mJumpHipDropPermitBeginFrame;
}
s32 PlayerConst::getJumpHipDropPermitEndFrame() const {
    return mJumpHipDropPermitEndFrame;
}
f32 PlayerConst::getHeadSlidingSpeed() const {
    return mHeadSlidingSpeed;
}
f32 PlayerConst::getHeadSlidingSpeedMin() const {
    return mHeadSlidingSpeedMin;
}
f32 PlayerConst::getHeadSlidingBrake() const {
    return mHeadSlidingBrake;
}
f32 PlayerConst::getHeadSlidingSideAccel() const {
    return mHeadSlidingSideAccel;
}
f32 PlayerConst::getHeadSlidingJump() const {
    return mHeadSlidingJump;
}
f32 PlayerConst::getHeadSlidingGravityAir() const {
    return mHeadSlidingGravityAir;
}
f32 PlayerConst::getSwimCenterOffset() const {
    return mSwimCenterOffset;
}
f32 PlayerConst::getSwimWallCatchOffset() const {
    return mSwimWallCatchOffset;
}
f32 PlayerConst::getSwimRisePower() const {
    return mSwimRisePower;
}
f32 PlayerConst::getSwimRiseSpeedMax() const {
    return mSwimRiseSpeedMax;
}
s32 PlayerConst::getSwimRiseFrame() const {
    return mSwimRiseFrame;
}
f32 PlayerConst::getSwimGravity() const {
    return mSwimGravity;
}
f32 PlayerConst::getSwimGravityWalk() const {
    return mSwimGravityWalk;
}
f32 PlayerConst::getSwimFallSpeedMax() const {
    return mSwimFallSpeedMax;
}
f32 PlayerConst::getSwimFloorAccelH() const {
    return mSwimFloorAccelH;
}
f32 PlayerConst::getSwimFloorSpeedMaxH() const {
    return mSwimFloorSpeedMaxH;
}
f32 PlayerConst::getSwimHighAccelH() const {
    return mSwimHighAccelH;
}
f32 PlayerConst::getSwimHighSpeedMaxH() const {
    return mSwimHighSpeedMaxH;
}
f32 PlayerConst::getSwimLowAccelH() const {
    return mSwimLowAccelH;
}
f32 PlayerConst::getSwimLowSpeedMaxH() const {
    return mSwimLowSpeedMaxH;
}
f32 PlayerConst::getSwimBrakeRateH() const {
    return mSwimBrakeRateH;
}
f32 PlayerConst::getSwimWallHitSpeedMinH() const {
    return mSwimWallHitSpeedMinH;
}
s32 PlayerConst::getSwimHighAccelPermitFrame() const {
    return mSwimHighAccelPermitFrame;
}
f32 PlayerConst::getSwimFlowFieldBlend() const {
    return mSwimFlowFieldBlend;
}
f32 PlayerConst::getSwimWalkAnimMinRate() const {
    return mSwimWalkAnimMinRate;
}
f32 PlayerConst::getSwimWalkAnimMaxRate() const {
    return mSwimWalkAnimMaxRate;
}
f32 PlayerConst::getSwimWalkMaxSpeed() const {
    return mSwimWalkMaxSpeed;
}
f32 PlayerConst::getSwimSpinCapUpPower() const {
    return mSwimSpinCapUpPower;
}
f32 PlayerConst::getSwimSpinCapUpSpeedMax() const {
    return mSwimSpinCapUpSpeedMax;
}
f32 PlayerConst::getSwimRotStartAngle() const {
    return mSwimRotStartAngle;
}
f32 PlayerConst::getSwimRotFastAngle() const {
    return mSwimRotFastAngle;
}
s32 PlayerConst::getSwimRotAccelFrame() const {
    return mSwimRotAccelFrame;
}
s32 PlayerConst::getSwimRotAccelFrameFast() const {
    return mSwimRotAccelFrameFast;
}
s32 PlayerConst::getSwimRotBrakeFrame() const {
    return mSwimRotBrakeFrame;
}
f32 PlayerConst::getSwimRotSpeedChangeStart() const {
    return mSwimRotSpeedChangeStart;
}
f32 PlayerConst::getSwimRotSpeedForward() const {
    return mSwimRotSpeedForward;
}
f32 PlayerConst::getSwimRotSpeedMax() const {
    return mSwimRotSpeedMax;
}
f32 PlayerConst::getSwimSurfaceAccelH() const {
    return mSwimSurfaceAccelH;
}
f32 PlayerConst::getSwimSurfaceSpeedMaxH() const {
    return mSwimSurfaceSpeedMaxH;
}
s32 PlayerConst::getSwimSurfaceSpinCapFrame() const {
    return mSwimSurfaceSpinCapFrame;
}
f32 PlayerConst::getSwimSurfaceSpinCapSpeedMaxH() const {
    return mSwimSurfaceSpinCapSpeedMaxH;
}
f32 PlayerConst::getSwimSurfaceStartDist() const {
    return mSwimSurfaceStartDist;
}
f32 PlayerConst::getSwimSurfaceEndDist() const {
    return mSwimSurfaceEndDist;
}
f32 PlayerConst::getSwimSurfaceGravity() const {
    return mSwimSurfaceGravity;
}
f32 PlayerConst::getSwimSurfaceBaseHeight() const {
    return mSwimSurfaceBaseHeight;
}
f32 PlayerConst::getSwimSurfaceSpring() const {
    return mSwimSurfaceSpring;
}
f32 PlayerConst::getSwimSurfaceDamper() const {
    return mSwimSurfaceDamper;
}
s32 PlayerConst::getSwimSurfaceDamperStart() const {
    return mSwimSurfaceDamperStart;
}
s32 PlayerConst::getSwimSurfaceDamperFrame() const {
    return mSwimSurfaceDamperFrame;
}
f32 PlayerConst::getSwimSurfaceEnableJumpHeight() const {
    return mSwimSurfaceEnableJumpHeight;
}
s32 PlayerConst::getSwimSurfacePreInputJumpFrame() const {
    return mSwimSurfacePreInputJumpFrame;
}
f32 PlayerConst::getSwimSurfaceMoveSpring() const {
    return mSwimSurfaceMoveSpring;
}
f32 PlayerConst::getSwimSurfaceMoveDamper() const {
    return mSwimSurfaceMoveDamper;
}
f32 PlayerConst::getSwimSurfaceMoveBaseHeight() const {
    return mSwimSurfaceMoveBaseHeight;
}
f32 PlayerConst::getSwimRunSurfaceBaseHeight() const {
    return mSwimRunSurfaceBaseHeight;
}
f32 PlayerConst::getSwimRunSurfaceApproachRate() const {
    return mSwimRunSurfaceApproachRate;
}
f32 PlayerConst::getSwimRunSurfaceApproachLimit() const {
    return mSwimRunSurfaceApproachLimit;
}
f32 PlayerConst::getSwimRunSurfaceBrakeBorder() const {
    return mSwimRunSurfaceBrakeBorder;
}
f32 PlayerConst::getSwimRunSurfaceBrakeH() const {
    return mSwimRunSurfaceBrakeH;
}
f32 PlayerConst::getSwimRunSurfaceApproachBorderMax() const {
    return mSwimRunSurfaceApproachBorderMax;
}
f32 PlayerConst::getSwimRunSurfaceApproachBorderMin() const {
    return mSwimRunSurfaceApproachBorderMin;
}
f32 PlayerConst::getSwimRunSurfaceApproachRateMin() const {
    return mSwimRunSurfaceApproachRateMin;
}
f32 PlayerConst::getSwimFallInSpeed() const {
    return mSwimFallInSpeed;
}
f32 PlayerConst::getSwimFallInBrakeH() const {
    return mSwimFallInBrakeH;
}
f32 PlayerConst::getSwimFallInBrakeV() const {
    return mSwimFallInBrakeV;
}
f32 PlayerConst::getSwimHeadInBrakeH() const {
    return mSwimHeadInBrakeH;
}
f32 PlayerConst::getSwimHeadInBrakeV() const {
    return mSwimHeadInBrakeV;
}
f32 PlayerConst::getSwimHeadInRisePower() const {
    return mSwimHeadInRisePower;
}
f32 PlayerConst::getSwimHeadInRiseSpeedMax() const {
    return mSwimHeadInRiseSpeedMax;
}
f32 PlayerConst::getSwimHeadInSurfaceHeight() const {
    return mSwimHeadInSurfaceHeight;
}
s32 PlayerConst::getSwimFallInForceSurfaceFrame() const {
    return mSwimFallInForceSurfaceFrame;
}
s32 PlayerConst::getSwimFallInvalidJumpFrame() const {
    return mSwimFallInvalidJumpFrame;
}
f32 PlayerConst::getSwimDiveStartSpeed() const {
    return mSwimDiveStartSpeed;
}
f32 PlayerConst::getSwimDiveBrake() const {
    return mSwimDiveBrake;
}
f32 PlayerConst::getSwimDiveEndSpeed() const {
    return mSwimDiveEndSpeed;
}
s32 PlayerConst::getSwimDiveLandCount() const {
    return mSwimDiveLandCount;
}
s32 PlayerConst::getSwimDiveLandCancelFrame() const {
    return mSwimDiveLandCancelFrame;
}
s32 PlayerConst::getSwimDiveNoBrakeFrame() const {
    return mSwimDiveNoBrakeFrame;
}
s32 PlayerConst::getSwimDiveButtonValidFrame() const {
    return mSwimDiveButtonValidFrame;
}
s32 PlayerConst::getSwimDiveEndFrame() const {
    return mSwimDiveEndFrame;
}
f32 PlayerConst::getSwimDiveInBrakeH() const {
    return mSwimDiveInBrakeH;
}
f32 PlayerConst::getSwimDiveInBrakeV() const {
    return mSwimDiveInBrakeV;
}
f32 PlayerConst::getSwimDiveInRisePower() const {
    return mSwimDiveInRisePower;
}
f32 PlayerConst::getSwimDiveInRiseSpeedMax() const {
    return mSwimDiveInRiseSpeedMax;
}
f32 PlayerConst::getSwimDiveInSurfaceHeight() const {
    return mSwimDiveInSurfaceHeight;
}
s32 PlayerConst::getSwimDiveInKeepFrame() const {
    return mSwimDiveInKeepFrame;
}
s32 PlayerConst::getSwimHeadSlidingFrame() const {
    return mSwimHeadSlidingFrame;
}
s32 PlayerConst::getSwimHeadSlidingBrakeFrame() const {
    return mSwimHeadSlidingBrakeFrame;
}
f32 PlayerConst::getSwimHeadSlidingSpeed() const {
    return mSwimHeadSlidingSpeed;
}
f32 PlayerConst::getSwimHeadSlidingSpeedEnd() const {
    return mSwimHeadSlidingSpeedEnd;
}
f32 PlayerConst::getSwimHeadSlidingBrake() const {
    return mSwimHeadSlidingBrake;
}
f32 PlayerConst::getSwimHeadSlidingSideAccel() const {
    return mSwimHeadSlidingSideAccel;
}
f32 PlayerConst::getSwimHeadSlidingJump() const {
    return mSwimHeadSlidingJump;
}
f32 PlayerConst::getSwimHeadSlidingGravity() const {
    return mSwimHeadSlidingGravity;
}
s32 PlayerConst::getSwimHeadSlidingEndBrakeFrame() const {
    return mSwimHeadSlidingEndBrakeFrame;
}
s32 PlayerConst::getSwimHeadSlidingEndSpeedMin() const {
    return mSwimHeadSlidingEndSpeedMin;
}
f32 PlayerConst::getSwimJumpHipDropSpeed() const {
    return mSwimJumpHipDropSpeed;
}
f32 PlayerConst::getSwimJumpHipDropBrakeV() const {
    return mSwimJumpHipDropBrakeV;
}
f32 PlayerConst::getSwimJumpHipDropBrakeVCeiling() const {
    return mSwimJumpHipDropBrakeVCeiling;
}
f32 PlayerConst::getSwimJumpHipDropGravity() const {
    return mSwimJumpHipDropGravity;
}
f32 PlayerConst::getSwimJumpHipDropCancelSpeed() const {
    return mSwimJumpHipDropCancelSpeed;
}
f32 PlayerConst::getSwimJumpHipDropAccelH() const {
    return mSwimJumpHipDropAccelH;
}
f32 PlayerConst::getSwimJumpHipDropMoveSpeedH() const {
    return mSwimJumpHipDropMoveSpeedH;
}
f32 PlayerConst::getSwimJumpHipDropPopSpeed() const {
    return mSwimJumpHipDropPopSpeed;
}
f32 PlayerConst::getSwimJumpHipDropPopJumpAdd() const {
    return mSwimJumpHipDropPopJumpAdd;
}
f32 PlayerConst::getSwimTramplePower() const {
    return mSwimTramplePower;
}
f32 PlayerConst::getDiveTramplePower() const {
    return mDiveTramplePower;
}
s32 PlayerConst::getDiveTrampleCancelFrame() const {
    return mDiveTrampleCancelFrame;
}
f32 PlayerConst::getDamageSwimPushPower() const {
    return mDamageSwimPushPower;
}
f32 PlayerConst::getDamageSwimGravity() const {
    return mDamageSwimGravity;
}
s32 PlayerConst::getDamageSwimCancelFrame() const {
    return mDamageSwimCancelFrame;
}
f32 PlayerConst::getDamageSwimSurfaceGravity() const {
    return mDamageSwimSurfaceGravity;
}
f32 PlayerConst::getDamageSwimSurfaceHopPower() const {
    return mDamageSwimSurfaceHopPower;
}
f32 PlayerConst::getDamageSwimSurfacePushPower() const {
    return mDamageSwimSurfacePushPower;
}
f32 PlayerConst::getDamageSwimSurfaceLandSpeed() const {
    return mDamageSwimSurfaceLandSpeed;
}
f32 PlayerConst::getDamageSwimSurfaceLandBrake() const {
    return mDamageSwimSurfaceLandBrake;
}
f32 PlayerConst::getDamageSwimSurfaceLandEndSpeed() const {
    return mDamageSwimSurfaceLandEndSpeed;
}
s32 PlayerConst::getDamageSwimSurfaceCancelFrame() const {
    return mDamageSwimSurfaceCancelFrame;
}
f32 PlayerConst::getDamageSwimBrakeRateGround() const {
    return mDamageSwimBrakeRateGround;
}
s32 PlayerConst::getOxygenReduceFrame() const {
    return mOxygenReduceFrame;
}
s32 PlayerConst::getOxygenNoReduceFrame() const {
    return mOxygenNoReduceFrame;
}
s32 PlayerConst::getOxygenRecoveryFrame() const {
    return mOxygenRecoveryFrame;
}
s32 PlayerConst::getOxygenDamageInterval() const {
    return mOxygenDamageinterval;
}
s32 PlayerConst::getIceWaterDamageInterval() const {
    return mIceWaterDamageinterval;
}
s32 PlayerConst::getIceWaterRecoveryFrame() const {
    return mIceWaterRecoveryFrame;
}
f32 PlayerConst::getMoveAnimSpeedMax() const {
    return mMoveAnimSpeedMax;
}
f32 PlayerConst::getAnimFrameRateSpeedMin() const {
    return mAnimFrameRateSpeedMin;
}
f32 PlayerConst::getRunBorderSpeed() const {
    return mRunBorderSpeed;
}
f32 PlayerConst::getRunBlendRange() const {
    return mRunBlendRange;
}
f32 PlayerConst::getDashBorderSpeed() const {
    return mDashBorderSpeed;
}
f32 PlayerConst::getDashBlendRange() const {
    return mDashBlendRange;
}
f32 PlayerConst::getDashFastBorderSpeed() const {
    return mDashFastBorderSpeed;
}
f32 PlayerConst::getDashFastBlendRange() const {
    return mDashFastBlendRange;
}
f32 PlayerConst::getAnimFrameRateSpeedMax() const {
    return mAnimFrameRateSpeedMax;
}
f32 PlayerConst::getAnimFrameRateRunStart() const {
    return mAnimFrameRateRunStart;
}
f32 PlayerConst::getAnimFrameRateMinRun() const {
    return mAnimFrameRateMinRun;
}
f32 PlayerConst::getAnimFrameRateMaxRun() const {
    return mAnimFrameRateMaxRun;
}
f32 PlayerConst::getAnimFrameRateMaxDash() const {
    return mAnimFrameRateMaxDash;
}
f32 PlayerConst::getAnimFrameRateMaxDashFast() const {
    return mAnimFrameRateMaxDashFast;
}
f32 PlayerConst::getRunStartPlayFrameScale() const {
    return mRunStartPlayFrameScale;
}
s32 PlayerConst::getRunStartBlendFrame() const {
    return mRunStartBlendFrame;
}
f32 PlayerConst::getDamageFireRunAnimRate() const {
    return mDamageFireRunAnimRate;
}
f32 PlayerConst::getRunSkateAnimSpeedOffset() const {
    return mRunSkateAnimSpeedOffset;
}
f32 PlayerConst::getAnimFrameRateRange2D() const {
    return mAnimFrameRateRange2D;
}
f32 PlayerConst::getAnimFrameRateMinRun2D() const {
    return mAnimFrameRateMinRun2D;
}
f32 PlayerConst::getAnimFrameRateMaxRun2D() const {
    return mAnimFrameRateMaxRun2D;
}
s32 PlayerConst::getIKBlendFrameRun() const {
    return mIKBlendFrameRun;
}
f32 PlayerConst::getIKBlendRateRunMin() const {
    return mIKBlendRateRunMin;
}
f32 PlayerConst::getIKBlendRateRunMax() const {
    return mIKBlendRateRunMax;
}
f32 PlayerConst::getRollingAnimBorderSpeedMin() const {
    return mRollingAnimBorderSpeedMin;
}
f32 PlayerConst::getRollingAnimBorderSpeedMax() const {
    return mRollingAnimBorderSpeedMax;
}
f32 PlayerConst::getRollingAnimFrameRateMin() const {
    return mRollingAnimFrameRateMin;
}
f32 PlayerConst::getRollingAnimFrameRateMax() const {
    return mRollingAnimFrameRateMax;
}
s32 PlayerConst::getSwimPaddleAnimInterval() const {
    return mSwimPaddleAniminterval;
}
s32 PlayerConst::getSwimPaddleAnimRateIntervalMax() const {
    return mSwimPaddleAnimRateintervalMax;
}
s32 PlayerConst::getSwimPaddleAnimRateIntervalMin() const {
    return mSwimPaddleAnimRateintervalMin;
}
f32 PlayerConst::getSwimPaddleAnimMaxRate() const {
    return mSwimPaddleAnimMaxRate;
}
f32 PlayerConst::getSwimBentForwardMax() const {
    return mSwimBentForwardMax;
}
f32 PlayerConst::getSwimBentForwardBlendRate() const {
    return mSwimBentForwardBlendRate;
}
f32 PlayerConst::getSwimBentSideMax() const {
    return mSwimBentSideMax;
}
f32 PlayerConst::getSwimBentSpineMax() const {
    return mSwimBentSpineMax;
}
f32 PlayerConst::getSwimBentSideBlendRate() const {
    return mSwimBentSideBlendRate;
}
f32 PlayerConst::getSwimBentFrontMax() const {
    return mSwimBentFrontMax;
}
f32 PlayerConst::getSwimBentFrontBlendRate() const {
    return mSwimBentFrontBlendRate;
}
f32 PlayerConst::getSwimWalkAnimSpeedMax() const {
    return mSwimWalkAnimSpeedMax;
}
f32 PlayerConst::getSwimWalkAnimSpeedMin() const {
    return mSwimWalkAnimSpeedMin;
}
f32 PlayerConst::getSwimWalkAnimFrameRateMax() const {
    return mSwimWalkAnimFrameRateMax;
}
f32 PlayerConst::getSwimWalkAnimFrameRateMin() const {
    return mSwimWalkAnimFrameRateMin;
}
f32 PlayerConst::getSandSinkBorderMin() const {
    return mSandSinkBorderMin;
}
f32 PlayerConst::getSandSinkBorderMax() const {
    return mSandSinkBorderMax;
}
f32 PlayerConst::getSandSinkBorderRateMin() const {
    return mSandSinkBorderRateMin;
}
f32 PlayerConst::getSandSinkBorderRateMax() const {
    return mSandSinkBorderRateMax;
}
f32 PlayerConst::getSandSinkFrameRateMin() const {
    return mSandSinkFrameRateMin;
}
f32 PlayerConst::getSandSinkFrameRateMax() const {
    return mSandSinkFrameRateMax;
}
s32 PlayerConst::getLookAtEyeKeepFrame() const {
    return mLookAtEyeKeepFrame;
}
s32 PlayerConst::getLookAtEyeKeepFrameInSight() const {
    return mLookAtEyeKeepFrameInSight;
}
s32 PlayerConst::getLookAtEyeKeepFrameWait() const {
    return mLookAtEyeKeepFrameWait;
}
f32 PlayerConst::getLookAtEyeDistance() const {
    return mLookAtEyeDistance;
}
f32 PlayerConst::getLookAtEyeAngleMinH() const {
    return mLookAtEyeAngleMinH;
}
f32 PlayerConst::getLookAtEyeAngleMinInSightH() const {
    return mLookAtEyeAngleMinInSightH;
}
f32 PlayerConst::getLookAtEyeAngleMaxH() const {
    return mLookAtEyeAngleMaxH;
}
f32 PlayerConst::getLookAtEyeAngleMinV() const {
    return mLookAtEyeAngleMinV;
}
f32 PlayerConst::getLookAtEyeAngleMinInSightV() const {
    return mLookAtEyeAngleMinInSightV;
}
f32 PlayerConst::getLookAtEyeAngleMaxV() const {
    return mLookAtEyeAngleMaxV;
}
f32 PlayerConst::getTiltEyeBorderStart() const {
    return mTiltEyeBorderStart;
}
f32 PlayerConst::getTiltEyeBorderEnd() const {
    return mTiltEyeBorderEnd;
}
f32 PlayerConst::getTiltEyeAngleScale() const {
    return mTiltEyeAngleScale;
}
f32 PlayerConst::getCenterTiltRateMax() const {
    return mCenterTiltRateMax;
}
f32 PlayerConst::getNoseChildLocalOffset() const {
    return mNoseChildLocalOffset;
}
f32 PlayerConst::getNoseStability() const {
    return mNoseStability;
}
f32 PlayerConst::getNoseFriction() const {
    return mNoseFriction;
}
f32 PlayerConst::getNoseLimitDegree() const {
    return mNoseLimitDegree;
}
f32 PlayerConst::getMustacheChildLocalOffset() const {
    return mMustacheChildLocalOffset;
}
f32 PlayerConst::getMustacheStability() const {
    return mMustacheStability;
}
f32 PlayerConst::getMustacheFriction() const {
    return mMustacheFriction;
}
f32 PlayerConst::getMustacheLimitDegree() const {
    return mMustacheLimitDegree;
}
s32 PlayerConst::getCapInterpolateFrame() const {
    return mCapinterpolateFrame;
}
f32 PlayerConst::getCapChildLocalOffset() const {
    return mCapChildLocalOffset;
}
f32 PlayerConst::getCapStability() const {
    return mCapStability;
}
f32 PlayerConst::getCapFriction() const {
    return mCapFriction;
}
f32 PlayerConst::getCapLimitDegree() const {
    return mCapLimitDegree;
}
f32 PlayerConst::getCapTransStability() const {
    return mCapTransStability;
}
f32 PlayerConst::getCapTransFriction() const {
    return mCapTransFriction;
}
f32 PlayerConst::getCapTransLimit() const {
    return mCapTransLimit;
}
s32 PlayerConst::getCapManHeroEyesWaitAppearFrame() const {
    return mCapManHeroEyesWaitAppearFrame;
}
s32 PlayerConst::getDeadWipeStartDamage() const {
    return mDeadWipeStartDamage;
}
s32 PlayerConst::getDeadWipeWaitDamage() const {
    return mDeadWipeWaitDamage;
}
s32 PlayerConst::getDeadWipeStartAbyss() const {
    return mDeadWipeStartAbyss;
}
s32 PlayerConst::getDeadWipeWaitAbyss() const {
    return mDeadWipeWaitAbyss;
}
s32 PlayerConst::getDeadWipeStartAbyssWithCapMsg() const {
    return mDeadWipeStartAbyssWithCapMsg;
}
s32 PlayerConst::getDeadWipeWaitAbyssWithCapMsg() const {
    return mDeadWipeWaitAbyssWithCapMsg;
}
s32 PlayerConst::getDeadWipeStartPress() const {
    return mDeadWipeStartPress;
}
s32 PlayerConst::getDeadWipeWaitPress() const {
    return mDeadWipeWaitPress;
}
s32 PlayerConst::getDeadWipeStartSandSink() const {
    return mDeadWipeStartSandSink;
}
s32 PlayerConst::getDeadWipeWaitSandSink() const {
    return mDeadWipeWaitSandSink;
}
s32 PlayerConst::getDeadWipeStartNoOxygen() const {
    return mDeadWipeStartNoOxygen;
}
s32 PlayerConst::getDeadWipeWaitNoOxygen() const {
    return mDeadWipeWaitNoOxygen;
}
s32 PlayerConst::getDeadWipeStartIceWater() const {
    return mDeadWipeStartIceWater;
}
s32 PlayerConst::getDeadWipeWaitIceWater() const {
    return mDeadWipeWaitIceWater;
}
f32 PlayerConst::getCoinDashSpeed() const {
    return mCoinDashSpeed;
}
f32 PlayerConst::getCoinDashSpeedLimit() const {
    return mCoinDashSpeedLimit;
}
f32 PlayerConst::getAdditionalSpeedLimit() const {
    return mAdditionalSpeedLimit;
}
