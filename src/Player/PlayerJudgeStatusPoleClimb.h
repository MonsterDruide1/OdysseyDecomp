#pragma once

#include "Player/IJudge.h"

class PlayerStatePoleClimb;

class PlayerJudgeStatusPoleClimb : public IJudge {
public:
    PlayerJudgeStatusPoleClimb(const IJudge* judge, const PlayerStatePoleClimb* state);

    void reset() override {}

    void update() override {}

    bool judge() const override;

private:
    const IJudge* mJudge = nullptr;
    const PlayerStatePoleClimb* mState = nullptr;
};

static_assert(sizeof(PlayerJudgeStatusPoleClimb) == 0x18);
