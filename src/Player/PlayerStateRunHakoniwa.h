#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "PlayerUtil.h"
#include "math/seadVector.h"

class PlayerAnimControlRun;
class PlayerActionPivotTurnControl;
class PlayerActionGroundMoveControl;
class PlayerJudgeWallPush;

class PlayerStateRunHakoniwa : public al::ActorStateBase {
public:
    PlayerStateRunHakoniwa(al::LiveActor*, PlayerConst const*, PlayerInput const*,
                           IUsePlayerCollision const*, PlayerCounterForceRun const*,
                           PlayerCounterQuickTurnJump const*, PlayerTrigger*, PlayerAnimator*,
                           PlayerEffect*, PlayerJointParamCenterDynamics*, IJudge const*, bool);
    ~PlayerStateRunHakoniwa() override;

    void appear() override;
    void kill() override;
    void control() override;

    bool isEnableLookAt() const;
    bool isRunDashFast() const;
    bool tryTurnJump(sead::Vector3f*);
    f32 getCenterTiltRateMax() const;
    f32 getCapDynamicsRate() const;

    void exePivot();
    void exeRun();
    void exeBrake();
    void exeTurn();
    void exeWallPush();

private:
    const PlayerConst* mConst;
    const PlayerInput* mInput;
    const IUsePlayerCollision* mCollision;
    const PlayerCounterForceRun* mCounterForceRun;
    const PlayerCounterQuickTurnJump* mCounterQuickTurnJump;
    PlayerTrigger* mTrigger;
    PlayerAnimator* mAnimator;
    PlayerAnimControlRun* mAnimControlRun = nullptr;
    PlayerActionPivotTurnControl* mActionPivotTurnControl = nullptr;
    PlayerActionGroundMoveControl* mActionGroundMoveControl = nullptr;
    bool _70 = false;
    sead::Vector3f _74 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _80 = {0.0f, 0.0f, 0.0f};
    float _8C = 0.0f;
    int _90 = 0;
    PlayerJudgeWallPush* mJudgeWallPush = nullptr;
    int _A0 = 0;
    int _A4 = 0;
    float val1 = 0.0f;
    float mTurnTiltRate = 0.0f;
    PlayerEffect* mEffect = nullptr;
    bool _B8 = 0;
    int _BC = 0;
    PlayerJointParamCenterDynamics* mJointDynamics;
    bool _C8 = false;
};
