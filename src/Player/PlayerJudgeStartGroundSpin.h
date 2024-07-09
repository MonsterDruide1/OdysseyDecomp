#pragma once

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}  // namespace al
class IUsePlayerCollision;
class PlayerInput;

class PlayerJudgeStartGroundSpin : public IJudge {
public:
    PlayerJudgeStartGroundSpin(const al::LiveActor* player, const IUsePlayerCollision* collider,
                               const PlayerInput* input);
    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const al::LiveActor* mPlayer;
    const IUsePlayerCollision* mCollider;
    const PlayerInput* mInput;
};

static_assert(sizeof(PlayerJudgeStartGroundSpin) == 0x20);
