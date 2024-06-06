#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Player/IJudge.h"
#include "Player/PlayerJudgeDeadWipeStart.h"

class PlayerJudgeDrawForward : public IJudge {
public:
    PlayerJudgeDrawForward(const al::LiveActor* playerActor,
                           const PlayerJudgeDeadWipeStart* playerJudgeDeadWipeStart);
    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const al::LiveActor* mPlayerActor;
    const PlayerJudgeDeadWipeStart* mPlayerJudgeDeadWipeStart;
};
