#pragma once

#include "Library/HostIO/HioNode.h"

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}
class IUsePlayerCollision;
class PlayerExternalVelocity;

class PlayerJudgeAirForceCount : public al::HioNode, public IJudge {
public:
    PlayerJudgeAirForceCount(const al::LiveActor* player,
                             const PlayerExternalVelocity* externalVelocity,
                             const IUsePlayerCollision* collider);

    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const al::LiveActor* mPlayer;
    const PlayerExternalVelocity* mExternalVelocity;
    const IUsePlayerCollision* mCollider;
    s32 mCounterAirForce = 0;
};
