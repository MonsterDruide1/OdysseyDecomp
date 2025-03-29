#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al
class PlayerConst;
class PlayerInput;
class IUsePlayerCollision;
class PlayerAnimator;

class PlayerStateNormalWallSlide : public al::ActorStateBase {
public:
    PlayerStateNormalWallSlide(al::LiveActor*, const PlayerConst*, const PlayerInput*,
                               IUsePlayerCollision*, PlayerAnimator*);

    void appear() override;
    void exeKeep();
    bool followNormal();
    void exeSlide();

    const sead::Vector3f& getNormalStart() const { return mNormalStart; }

private:
    const PlayerConst* mConst;
    const PlayerInput* mInput;
    IUsePlayerCollision* mCollision;
    PlayerAnimator* mAnimator;
    sead::Vector3f mNormalStart = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mNormalEnd = {0.0f, 0.0f, 0.0f};
    f32 _58 = 0;
    s32 _5c = 0;
    bool _60 = 0;
};
