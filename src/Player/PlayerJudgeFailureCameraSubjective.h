#pragma once

#include "Player/IJudge.h"

class PlayerInput;

class PlayerJudgeFailureCameraSubjective : public IJudge {
public:
    PlayerJudgeFailureCameraSubjective(const PlayerInput* input, const IJudge* inCameraNerveJudge);

    bool judge() const override;

    void reset() override {}

    void update() override {}

private:
    const PlayerInput* mInput = nullptr;
    const IJudge* mInCameraNerveJudge = nullptr;
};

static_assert(sizeof(PlayerJudgeFailureCameraSubjective) == 0x18);
