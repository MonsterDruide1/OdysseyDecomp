#pragma once

#include "Player/IJudge.h"

class PlayerStateAbyss;

class PlayerJudgeSafetyPointRecovery : public IJudge {
public:
    PlayerJudgeSafetyPointRecovery(IJudge* judge, const PlayerStateAbyss* state);

    void reset() override {}

    void update() override {}

    bool judge() const override;

private:
    IJudge* mJudge = nullptr;
    const PlayerStateAbyss* mState = nullptr;
};

static_assert(sizeof(PlayerJudgeSafetyPointRecovery) == 0x18);
