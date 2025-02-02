#pragma once

#include "Library/Nerve/NerveStateBase.h"

class PlayerConst;
class PlayerInput;
class IUsePlayerCollision;
class PlayerAnimator;
class PlayerActionSlopeSlideControl;

class PlayerStateSlope : public al::ActorStateBase {
public:
    PlayerStateSlope(al::LiveActor* player, const PlayerConst* pConst, const PlayerInput* input,
                     const IUsePlayerCollision* collision, PlayerAnimator* animator);

    void appear() override;
    void kill() override;
    bool isEnableCancelSandSink() const;
    f32 calcSlideSpeed() const;

    void exeSlide();

private:
    const PlayerConst* mConst;
    const IUsePlayerCollision* mCollision;
    PlayerAnimator* mAnimator;
    PlayerActionSlopeSlideControl* mActionSlopeSlideControl = 0;
    bool mIsForceSlide = 0;
    bool mIsRunningRumbleLoop = 0;
    s32 mTimeInAir = 0;
    s32 mSlopeForceCounter = 0;
};
