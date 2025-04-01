#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

class IJudge;
class IUsePlayerCollision;
class PlayerActionDiveInWater;
class PlayerAnimator;
class PlayerJudgePreInputJump;
class PlayerWallActionHistory;
class PlayerStateNormalWallSlide;
class PlayerStateNormalWallJump;
class PlayerConst;
class PlayerInput;
class PlayerTrigger;

class PlayerStateWallAir : public al::ActorStateBase {
public:
    PlayerStateWallAir(al::LiveActor* player, const PlayerConst* pConst, const PlayerInput* input,
                       const PlayerTrigger* trigger, IUsePlayerCollision* collision,
                       IJudge* judgeWallKeep, PlayerJudgePreInputJump* judgePreInputJump,
                       PlayerAnimator* animator, PlayerWallActionHistory* wallActionHistory,
                       PlayerActionDiveInWater* actionDiveInWater);

    void appear() override;
    bool isAir() const;
    bool isJustJump() const;
    bool isEnableReactionCapCatch() const;
    void startSlideSpinAttack();
    void calcSnapMoveCutDir(sead::Vector3f* cutDir) const;
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
