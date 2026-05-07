#pragma once

#include "Player/IJudge.h"

class PlayerStateCameraSubjective;

class PlayerJudgeActiveCameraSubjective : public IJudge {
public:
    PlayerJudgeActiveCameraSubjective(const PlayerStateCameraSubjective* state);

    void reset() override {}

    void update() override {}

    bool judge() const override;

private:
    const PlayerStateCameraSubjective* mState = nullptr;
};

static_assert(sizeof(PlayerJudgeActiveCameraSubjective) == 0x10);
