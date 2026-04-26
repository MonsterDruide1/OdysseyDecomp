#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}

class PlayerConst;
class PlayerRecoverySafetyPoint;
class PlayerColliderHakoniwa;
class PlayerAnimator;
class PlayerStateRecoveryDead;

class PlayerStateAbyss : public al::ActorStateBase {
public:
    PlayerStateAbyss(al::LiveActor* player, const PlayerConst* playerConst,
                     PlayerRecoverySafetyPoint* recoverySafetyPoint,
                     PlayerColliderHakoniwa* playerCollider, PlayerAnimator* playerAnimator,
                     al::LiveActor* anotherActor);

    void appear() override;
    void kill() override;

    void exeFall();
    void exeRecovery();
    bool isRecovery() const;
    bool isRecoveryLandFall() const;
    void prepareRecovery();

private:
    const PlayerConst* mPlayerConst = nullptr;
    PlayerRecoverySafetyPoint* mPlayerRecoverySafetyPoint = nullptr;
    PlayerAnimator* mPlayerAnimator = nullptr;
    PlayerStateRecoveryDead* mPlayerStateRecoveryDead = nullptr;
};

static_assert(sizeof(PlayerStateAbyss) == 0x40);
