#pragma once

#include <math/seadVector.h>

#include "Player/HackerStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

class IUsePlayerCollision;
class IUsePlayerHack;
class PlayerActionAirMoveControl;
class PlayerAnimator;
class PlayerConst;

class YoshiStateHackDown : public HackerStateBase {
public:
    YoshiStateHackDown(al::LiveActor* player, IUsePlayerHack** hacker, const PlayerConst* pConst,
                       const IUsePlayerCollision* collider, PlayerAnimator* animator);

    void appear() override;
    bool isLand() const;
    bool isEnableCancel() const;
    void exeDown();
    void exeLand();

private:
    const PlayerConst* mConst = nullptr;
    const IUsePlayerCollision* mCollider = nullptr;
    PlayerAnimator* mAnimator = nullptr;
    PlayerActionAirMoveControl* mAirMoveControl = nullptr;
    sead::Vector3f mUp = {0.0f, 0.0f, 0.0f};
};

static_assert(sizeof(YoshiStateHackDown) == 0x58);
