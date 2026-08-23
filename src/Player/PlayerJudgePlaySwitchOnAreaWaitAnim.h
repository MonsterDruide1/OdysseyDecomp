#pragma once

#include "Player/IJudge.h"

class PlayerStateWait;

class PlayerJudgePlaySwitchOnAreaWaitAnim : public IJudge {
public:
    PlayerJudgePlaySwitchOnAreaWaitAnim(const PlayerStateWait* stateWait);

    void reset() override {}

    void update() override {}

    bool judge() const override;

private:
    const PlayerStateWait* mStateWait = nullptr;
};

static_assert(sizeof(PlayerJudgePlaySwitchOnAreaWaitAnim) == 0x10);
