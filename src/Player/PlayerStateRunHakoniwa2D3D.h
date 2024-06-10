#pragma once

#include "Library/Nerve/NerveStateBase.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerCounterForceRun.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerStateGroundSpin.h"
#include "PlayerUtil.h"
#include "Util/IUseDimension.h"
#include "playerUtil.h"

class PlayerStateRunHakoniwa;
class PlayerStateGroundSpin;
class PlayerStateRun2D;
class PlayerJudgeStartGroundSpin;
class PlayerJudgeWaterSurfaceRun;

class PlayerStateRunHakoniwa2D3D : public al::ActorStateBase {
public:
    PlayerStateRunHakoniwa2D3D(al::LiveActor *,PlayerConst const*,IUseDimension const*,PlayerInput const*,IUsePlayerCollision const*,al::WaterSurfaceFinder const*,PlayerCounterForceRun const*,PlayerCounterQuickTurnJump const*,PlayerTrigger *,PlayerAnimator *,PlayerEffect *,PlayerJointParamCenterDynamics *,bool);
    ~PlayerStateRunHakoniwa2D3D() override;
    
    void appear() override;
    bool update() override;
    bool isEnableLookAt() const;
    bool isGroundSpin() const;
    bool isSpinClockwise() const;
    bool isRunDashFast() const;
    bool isRunWaterSurface() const;
    bool isBrake2D() const;
    bool tryTurnJump(IJudge *,sead::Vector3<float> *);
    f32 getTurnTiltRate() const;
    f32 getCenterTiltRate() const;
    f32 getInverseKinematicsRate() const;
    f32 getCapDynamicsRate() const;

    void exeRun3D();
    void exeGroundSpin3D();
    void exeRun2D();

public:
    const PlayerConst* mConst;
    const PlayerInput* mInput;
    const IUseDimension* mDimension;
    const IUsePlayerCollision* mCollision;
    const al::WaterSurfaceFinder* mWaterSurfaceFinder;
    const PlayerCounterForceRun* mCounterForceRun;
    PlayerStateRunHakoniwa* mStateRunHakoniwa = nullptr;
    PlayerStateGroundSpin* mStateGroundSpin = nullptr;
    PlayerStateRun2D* mStateRun2D = nullptr;
    PlayerJudgeStartGroundSpin* mJudgeStartGroundSpin = nullptr;
    PlayerJudgeWaterSurfaceRun* mJudgeWaterSurfaceRun = nullptr;
};
