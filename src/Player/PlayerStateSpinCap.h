#pragma once

#include "Library/Nerve/NerveStateBase.h"
#include "math/seadVector.h"

namespace al {
class WaterSurfaceFinder;
}
class PlayerConst;
class PlayerInput;
class PlayerCounterForceRun;
class IUsePlayerCollision;
class PlayerTrigger;
class PlayerSpinCapAttack;
class PlayerAnimator;
class PlayerJointParamCapThrow;
class PlayerActionAirMoveControl;
class PlayerActionGroundMoveControl;
class PlayerJudgeWaterSurfaceRun;

class PlayerStateSpinCap : public al::ActorStateBase {
public:
    PlayerStateSpinCap(al::LiveActor*, PlayerConst const*, PlayerInput const*,
                       PlayerCounterForceRun const*, al::WaterSurfaceFinder const*,
                       IUsePlayerCollision const*, PlayerTrigger*, PlayerSpinCapAttack*,
                       PlayerAnimator*, PlayerJointParamCapThrow*);
    ~PlayerStateSpinCap() override;
    void appear() override;
    void kill() override;
    bool update() override;
    void control() override;
    bool noticeInWater();
    void resetJoint();
    void cancelPoseInterpole();
    bool isEnableCancelHipDrop() const;
    bool isEnableCancelAir() const;
    bool isSpinAttackAir() const;
    bool isEnableCancelGround() const;
    bool isEnableCancelCarry() const;
    bool isEnableReThrowCap() const;
    bool isWaterSurfaceRun() const;
    void exeSpinGround();
    void updateSpinGroundNerve();
    void exeSpinGroundSeparate();
    void exeSpinAir();
    void updateSpinAirNerve();
    void exeSpinAirSeparate();
    void exeFall();

public:
    const PlayerConst* mConst;
    const PlayerInput* mInput;
    const PlayerCounterForceRun* mCounterForceRun;
    const al::WaterSurfaceFinder* mWaterSurfaceFinder;
    const IUsePlayerCollision* mCollider;
    PlayerTrigger* mTrigger;
    PlayerSpinCapAttack* mSpinCapAttack;
    PlayerAnimator* mAnimator;
    PlayerJointParamCapThrow* mJointParamCapThrow;
    PlayerActionAirMoveControl* mActionAirMoveControl = 0;
    PlayerActionGroundMoveControl* mActionGroundMoveControl = 0;
    bool _78 = 0;
    sead::Vector3f _7C = {0.0f, 0.0f, 0.0f};
    bool _80 = 0;
    PlayerJudgeWaterSurfaceRun* mJudgeWaterSurfaceRun = 0;
    bool mIsInWater = 0;
    bool _99 = 0;
    sead::Vector3f _9C = {0.0f, 0.0f, 0.0f};
    bool _A8 = 0;
    bool _A9 = 0;
};
