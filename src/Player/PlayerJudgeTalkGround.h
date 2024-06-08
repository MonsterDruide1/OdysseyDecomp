#pragma once

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}

class IPlayerModelChanger;
class PlayerHackKeeper;
class PlayerCarryKeeper;
class IUsePlayerCollision;
class PlayerInput;
class PlayerConst;
class PlayerStateWait;

class PlayerJudgeTalkGround : public IJudge {
public:
    PlayerJudgeTalkGround(const al::LiveActor* playerActor,
                          const IPlayerModelChanger* playerModelChanger,
                          const PlayerHackKeeper* playerHackKeeper,
                          const PlayerCarryKeeper* playerCarryKeeper,
                          const IUsePlayerCollision* mPlayerCollision,
                          const PlayerInput* playerInput, const PlayerConst* playerConst,
                          const PlayerStateWait* playerStateWait);

    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const al::LiveActor* mPlayerActor;
    const IPlayerModelChanger* mPlayerModelChanger;
    const PlayerHackKeeper* mPlayerHackKeeper;
    const PlayerCarryKeeper* mPlayerCarryKeeper;
    const IUsePlayerCollision* mPlayerCollision;
    const PlayerInput* mPlayerInput;
    const PlayerConst* mPlayerConst;
    const PlayerStateWait* mPlayerStateWait;
};
