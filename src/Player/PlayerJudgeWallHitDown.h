#pragma once

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}
class IUsePlayerCollision;
class PlayerConst;
class PlayerTrigger;

class PlayerJudgeWallHitDown : public IJudge {
public:
    PlayerJudgeWallHitDown(const al::LiveActor* player, const IUsePlayerCollision* collider,
                           const PlayerConst* pConst, const PlayerTrigger* trigger);
    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const al::LiveActor* mPlayer;
    const IUsePlayerCollision* mCollider;
    const PlayerConst* mConst;
    const PlayerTrigger* mTrigger;
};
