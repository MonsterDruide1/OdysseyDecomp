#pragma once

#include "Library/Nerve/NerveStateBase.h"

class PlayerActionDiveInWater;
class PlayerAnimator;
class PlayerConst;
class PlayerInput;
class IUsePlayerCollision;

class PlayerStateHeadSliding : public al::ActorStateBase {
public:
    PlayerStateHeadSliding(al::LiveActor* player, const PlayerConst* pConst,
                           const IUsePlayerCollision* collision, const PlayerInput* input,
                           const PlayerActionDiveInWater* actionDiveInWater,
                           PlayerAnimator* animator);

    void appear() override;
    void kill() override;

    bool isEnableDiveInWater() const;

    void exeDive();

private:
    const PlayerConst* mConst;
    const IUsePlayerCollision* mCollision;
    const PlayerInput* mInput;
    const PlayerActionDiveInWater* mActionDiveInWater;
    PlayerAnimator* mAnimator;
    bool mIsEnableDiveInWater = false;
};
