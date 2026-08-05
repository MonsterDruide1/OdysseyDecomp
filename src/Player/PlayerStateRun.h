#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

class IJudge;
class IUsePlayerCollision;
class PlayerActionGroundMoveControl;
class PlayerActionPivotTurnControl;
class PlayerAnimator;
class PlayerConst;
class PlayerInput;

class PlayerStateRun : public al::ActorStateBase {
public:
    PlayerStateRun(al::LiveActor* player, const PlayerConst* pConst, const PlayerInput* input,
                   const IUsePlayerCollision* collision, PlayerAnimator* animator,
                   IJudge* judgeStartDash);

    void appear() override;

    bool tryTurnJump(IJudge* judge);
    void exePivot();
    void exeRun();
    bool tryChangeRunAnim(const char* animName);
    void exeBrake();
    void exeTurn();

private:
    const PlayerConst* mConst = nullptr;
    const PlayerInput* mInput = nullptr;
    const IUsePlayerCollision* mCollision = nullptr;
    PlayerAnimator* mAnimator = nullptr;
    PlayerActionPivotTurnControl* mPivotTurnControl = nullptr;
    PlayerActionGroundMoveControl* mGroundMoveControl = nullptr;
    u8 _50[4] = {};
    bool mIsEnableTurnJump = false;
    u8 _55[3] = {};
};

static_assert(sizeof(PlayerStateRun) == 0x58);
