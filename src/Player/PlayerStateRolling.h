#pragma once

#include "Library/Nerve/NerveStateBase.h"
#include "Player/PlayerActionAirMoveControl.h"
#include "Player/PlayerConst.h"
#include "PlayerUtil.h"

class PlayerStateRolling : public al::ActorStateBase {
public:
    PlayerStateRolling(al::LiveActor*, PlayerConst const*, PlayerInput const*,
                       IUsePlayerCollision const*, PlayerTrigger*, PlayerAnimator*, PlayerEffect*,
                       PlayerJudgeStartRolling*, IJudge*, PlayerJudgePreInputJump*,
                       PlayerJudgePreInputCapThrow*, IJudge*, PlayerContinuousLongJump*,
                       PlayerSeCtrl*);
    ~PlayerStateRolling() override;
    void appear() override;
    void kill() override;
    bool update() override;
    void control() override;

    bool isRolling() const;
    bool isRollingJump() const;
    bool isEnableCancelNormalJump() const;
    bool isEndSquat() const;
    bool isEndStandUp() const;
    bool isEnableTrample(al::HitSensor const*, al::HitSensor const*) const;
    f32 getInverseKinematicsRate() const;
    void exeBoostStart();
    void updateRollingAnimFrameRate();
    void exeStart();
    bool isStartRollingBrake() const;
    void exeRolling();
    void restartRolling(bool, bool);
    void exeJump();
    void exeLand();
    void exeUnRoll();
    void exeBrake();
    void exeStandUp();
    void exeEndSquat();

public:
    const PlayerConst* mConst;
    const PlayerInput* mInput;
    const IUsePlayerCollision* mCollider;
    PlayerTrigger* mTrigger;
    PlayerAnimator* mAnimator;
    PlayerEffect* mEffect;
    PlayerActionAirMoveControl* mActionAirMoveControl = nullptr;
    PlayerActionSlopeSlideControl* mActionSlopeSlideControl = nullptr;
    PlayerContinuousLongJump* mContinuousLongJump;
    PlayerJudgeStartRolling* mJudgeStartRolling;
    IJudge* mJudgeEnableStandUp;
    PlayerJudgePreInputJump* mJudgePreInputJump;
    PlayerJudgePreInputCapThrow* mJudgePreInputCapThrow;
    IJudge* mJudgeForceRolling;
    int _90 = 0;
    int _94 = 0;
    int _98 = 0;
    int _9C = 0;
    int _A0 = 0;
    PlayerSeCtrl* mSeCtrl;
    bool _B0 = false;
};
