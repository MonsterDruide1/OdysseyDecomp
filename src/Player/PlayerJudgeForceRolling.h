#pragma once

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}
class IUsePlayerCollision;

class PlayerJudgeForceRolling : public IJudge {
public:
    PlayerJudgeForceRolling(const al::LiveActor* player, const IUsePlayerCollision* collider);
    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const al::LiveActor* mPlayer;
    const IUsePlayerCollision* mCollider;
};
