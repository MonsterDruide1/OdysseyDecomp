#pragma once

#include "CUSTOM/PlayerAnimator.h"
#include "Library/Nerve/NerveStateBase.h"
#include "PlayerUtil.h"
#include "Stuff.h"

class PlayerStateNormalWallSlide : public al::ActorStateBase {
public:
    PlayerStateNormalWallSlide(al::LiveActor *,PlayerConst const*,PlayerInput const*,IUsePlayerCollision *,PlayerAnimator *);
    ~PlayerStateNormalWallSlide() override;

    void appear() override;
    void exeKeep();
    bool followNormal();
    void exeSlide();

private:
    const PlayerConst* mConst;
    const PlayerInput* mInput;
    IUsePlayerCollision* mCollision;
    PlayerAnimator* mAnimator;
    sead::Vector3f mNormalStart = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mNormalEnd = {0.0f, 0.0f, 0.0f};
    float _58 = 0;
    int _5C = 0;
    bool _60 = 0;
};
