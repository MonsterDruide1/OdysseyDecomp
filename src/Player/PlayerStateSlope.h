#pragma once

#include "Library/Nerve/NerveStateBase.h"

class IUsePlayerCollision;
class PlayerActionSlopeSlideControl;
class PlayerAnimator;
class PlayerConst;
class PlayerInput;

namespace al {
class LiveActor;
}  // namespace al

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
    PlayerActionSlopeSlideControl* mActionSlopeSlideControl = nullptr;
    bool mIsForceSlide = false;
    bool mIsRunningRumbleLoop = false;
    s32 mTimeInAir = 0;
    s32 mSlopeForceCounter = 0;
};
