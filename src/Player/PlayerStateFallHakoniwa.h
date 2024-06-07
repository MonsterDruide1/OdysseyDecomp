#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class AreaObj;
}

class PlayerConst;
class IUsePlayerCollision;
class PlayerInput;
class PlayerTrigger;
class PlayerAreaChecker;
class IUseDimension;
class PlayerAnimator;
class PlayerJudgeInvalidateInputFall;
class PlayerActionAirMoveControl;

class PlayerStateFallHakoniwa : public al::ActorStateBase {
public:
    PlayerStateFallHakoniwa(al::LiveActor*, const PlayerConst*, const IUsePlayerCollision*,
                            const PlayerInput*, const PlayerTrigger*, const PlayerAreaChecker*,
                            const IUseDimension*, PlayerAnimator*);
    ~PlayerStateFallHakoniwa() override;

    void appear() override;
    void updateFallTargetArea();
    bool isValidWallCatch();
    void exeFall();

public:
    const PlayerConst* mConst;
    const IUsePlayerCollision* mCollision;
    const PlayerTrigger* mTrigger;
    const IUseDimension* mDimension;
    PlayerAnimator* mAnimator;
    PlayerJudgeInvalidateInputFall* mJudgeInvalidateInputFall = nullptr;
    PlayerActionAirMoveControl* mAirMoveControl = nullptr;
    int gap1 = 0;
    int gap2 = 0;
    sead::Vector3f unk = {0.0f, 0.0f, 0.0f};
    bool flag1 = false;
    bool flag2 = false;
    al::AreaObj* mInvalidateInputFallArea = nullptr;
    float mMaxMoveSpeed = 0.0f;
};
