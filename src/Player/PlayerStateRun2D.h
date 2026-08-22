#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

class IUsePlayerCollision;
class PlayerActionGroundMoveControl;
class PlayerAnimator;
class PlayerConst;
class PlayerInput;

class PlayerStateRun2D : public al::ActorStateBase {
public:
    PlayerStateRun2D(al::LiveActor* player, const PlayerConst* pConst, const PlayerInput* input,
                     const IUsePlayerCollision* collision, PlayerAnimator* animator);

    void appear() override;

    bool isBrake() const;
    void exeRun();
    void exeBrake();
    void exeTurn();

private:
    const PlayerConst* mConst = nullptr;
    const PlayerInput* mInput = nullptr;
    const IUsePlayerCollision* mCollision = nullptr;
    PlayerAnimator* mAnimator = nullptr;
    PlayerActionGroundMoveControl* mGroundMoveControl = nullptr;
};

static_assert(sizeof(PlayerStateRun2D) == 0x48);
