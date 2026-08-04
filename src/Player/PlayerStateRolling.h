#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class HitSensor;
class LiveActor;
}  // namespace al

class IJudge;
class IUsePlayerCollision;
class PlayerAnimator;
class PlayerConst;
class PlayerContinuousLongJump;
class PlayerEffect;
class PlayerInput;
class PlayerJudgePreInputCapThrowSL;
class PlayerJudgePreInputJump;
class PlayerJudgeStartRolling;
class PlayerSeCtrl;
class PlayerTrigger;

class PlayerStateRolling : public al::ActorStateBase {
public:
    PlayerStateRolling(al::LiveActor* player, const PlayerConst* playerConst,
                       const PlayerInput* playerInput, const IUsePlayerCollision* playerCollision,
                       PlayerTrigger* playerTrigger, PlayerAnimator* playerAnimator,
                       PlayerEffect* playerEffect, PlayerJudgeStartRolling* judgeStartRolling,
                       IJudge* judgeEnableStandUp, PlayerJudgePreInputJump* judgePreInputJump,
                       PlayerJudgePreInputCapThrowSL* judgePreInputCapThrow,
                       IJudge* judgeForceRolling, PlayerContinuousLongJump* continuousLongJump,
                       PlayerSeCtrl* playerSeCtrl);

    void appear() override;
    void kill() override;
    bool update() override;
    void control() override;

    bool isRolling() const;
    bool isRollingJump() const;
    bool isEnableCancelNormalJump() const;
    bool isEndSquat() const;
    bool isEndStandUp() const;
    bool isEnableTrample(const al::HitSensor* sender, const al::HitSensor* receiver) const;
    f32 getInverseKinematicsRate() const;

    void exeBoostStart();
    void updateRollingAnimFrameRate();
    void exeStart();
    bool isStartRollingBrake() const;
    void exeRolling();
    void restartRolling(bool isRestart, bool isForce);
    void exeJump();
    void exeLand();
    void exeUnRoll();
    void exeBrake();
    void exeStandUp();
    void exeEndSquat();

private:
    void* padding[0x13];
};

static_assert(sizeof(PlayerStateRolling) == 0xb8);
