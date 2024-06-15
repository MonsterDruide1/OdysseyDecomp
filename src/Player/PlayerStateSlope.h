#pragma once

#include "Library/Nerve/NerveStateBase.h"

class PlayerConst;
class PlayerInput;
class IUsePlayerCollision;
class PlayerAnimator;
class PlayerActionSlopeSlideControl;

class PlayerStateSlope : public al::ActorStateBase {
public:
    PlayerStateSlope(al::LiveActor *,PlayerConst const*,PlayerInput const*,IUsePlayerCollision const*,PlayerAnimator *);
    ~PlayerStateSlope() override;

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
    int mTimeInAir = 0;
    int mSlopeForceCounter = 0;
};
