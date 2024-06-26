#pragma once

#include "Library/Nerve/NerveStateBase.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerStateNormalWallJump.h"
#include "Player/PlayerStateNormalWallSlide.h"
#include "Player/PlayerTrigger.h"
#include "math/seadVector.h"

class IJudge;
class IUsePlayerCollision;
class PlayerActionDiveInWater;
class PlayerAnimator;
class PlayerJudgePreInputJump;
class PlayerWallActionHistory;

class PlayerStateWallAir : public al::ActorStateBase {
public:
    PlayerStateWallAir(al::LiveActor*, PlayerConst const*, PlayerInput const*, PlayerTrigger const*,
                       IUsePlayerCollision*, IJudge*, PlayerJudgePreInputJump*, PlayerAnimator*,
                       PlayerWallActionHistory*, PlayerActionDiveInWater*);
    ~PlayerStateWallAir() override;
    void appear() override;
    bool isAir() const;
    bool isJustJump() const;
    bool isEnableReactionCapCatch() const;
    void startSlideSpinAttack();
    void calcSnapMoveCutDir(sead::Vector3f*) const;
    void exeSlide();
    void exeJump();

private:
    const PlayerConst* mConst;
    PlayerStateNormalWallSlide* mStateNormalWallSlide = nullptr;
    PlayerStateNormalWallJump* mStateNormalWallJump = nullptr;
    IJudge* mJudgeWallKeep;
    PlayerJudgePreInputJump* mJudgePreInputJump;
    const IUsePlayerCollision* mCollision;
    PlayerWallActionHistory* mWallActionHistory;
};
