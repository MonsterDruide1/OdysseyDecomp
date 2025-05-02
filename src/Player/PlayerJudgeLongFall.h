#pragma once

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}
class IPlayerModelChanger;
class IUsePlayerFallDistanceCheck;
class PlayerBindKeeper;
class PlayerConst;
class PlayerHackKeeper;

class PlayerJudgeLongFall : public IJudge {
public:
    PlayerJudgeLongFall(const al::LiveActor* player, const PlayerConst* pConst,
                        const IUsePlayerFallDistanceCheck* fallDistanceCheck,
                        const PlayerHackKeeper* hackKeeper, const PlayerBindKeeper* bindKeeper,
                        const IPlayerModelChanger* modelChanger);
    void reset() override {}
    void update() override {}
    bool judge() const override;

private:
    const al::LiveActor* mPlayer;
    const PlayerConst* mConst;
    const IUsePlayerFallDistanceCheck* mFallDistanceCheck;
    const PlayerHackKeeper* mHackKeeper;
    const PlayerBindKeeper* mBindKeeper;
    const IPlayerModelChanger* mModelChanger;
};

static_assert(sizeof(PlayerJudgeLongFall) == 0x38);
