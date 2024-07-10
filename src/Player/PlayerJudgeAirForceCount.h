#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}
class PlayerExternalVelocity;
class IUsePlayerCollision;

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
