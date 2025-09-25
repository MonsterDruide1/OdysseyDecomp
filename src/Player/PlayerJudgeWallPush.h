#pragma once

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}
class IUsePlayerCollision;
class PlayerInput;

class PlayerJudgeWallPush : public IJudge {
public:
    PlayerJudgeWallPush(const al::LiveActor* player, const IUsePlayerCollision* collider,
                        const PlayerInput* input);
    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const al::LiveActor* mPlayer;
    const IUsePlayerCollision* mCollider;
    const PlayerInput* mInput;
};
