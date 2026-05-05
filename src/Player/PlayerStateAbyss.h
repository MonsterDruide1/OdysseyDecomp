#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

class PlayerAnimator;
class PlayerColliderHakoniwa;
class PlayerConst;
class PlayerRecoverySafetyPoint;
class PlayerStateRecoveryDead;

class PlayerStateAbyss : public al::ActorStateBase {
public:
    PlayerStateAbyss(al::LiveActor* player, const PlayerConst* pConst,
                     PlayerRecoverySafetyPoint* recoverySafetyPoint,
                     PlayerColliderHakoniwa* collider, PlayerAnimator* animator,
                     al::LiveActor* capActor);

    void appear() override;
    void kill() override;
    void prepareRecovery();
    bool isRecovery() const;
    bool isRecoveryLandFall() const;
    void exeFall();
    void exeRecovery();

private:
    const PlayerConst* mConst = nullptr;
    PlayerRecoverySafetyPoint* mRecoverySafetyPoint = nullptr;
    PlayerAnimator* mAnimator = nullptr;
    PlayerStateRecoveryDead* mStateRecoveryDead = nullptr;
};

static_assert(sizeof(PlayerStateAbyss) == 0x40);
