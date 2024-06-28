#pragma once

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}
class IUsePlayerCollision;
class PlayerConst;
class PlayerCounterForceRun;
class PlayerTrigger;

class PlayerJudgeWallHitDownForceRun : public IJudge {
public:
    PlayerJudgeWallHitDownForceRun(const al::LiveActor* player, const IUsePlayerCollision* collider,
                           const PlayerConst* pConst, const PlayerCounterForceRun* counterForceRun, const PlayerTrigger* trigger);
    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const al::LiveActor* mPlayer;
    const IUsePlayerCollision* mCollider;
    const PlayerConst* mConst;
    const PlayerCounterForceRun* mCounterForceRun;
    const PlayerTrigger* mTrigger;
};
