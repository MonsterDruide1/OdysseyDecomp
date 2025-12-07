#pragma once

#include "Player/IJudge.h"

class PlayerTrigger;

class PlayerJudgeForceLand : public IJudge {
public:
    PlayerJudgeForceLand(const IJudge* judgeLongFall, const PlayerTrigger* trigger);

    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const IJudge* mJudgeLongFall;
    const PlayerTrigger* mTrigger;
};

static_assert(sizeof(PlayerJudgeForceLand) == 0x18);
