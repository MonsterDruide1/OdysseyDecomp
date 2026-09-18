#pragma once

#include <basis/seadTypes.h>

#include "Player/IJudge.h"

class PlayerInput;

class PlayerJudgeStartDash : public IJudge {
public:
    PlayerJudgeStartDash(const PlayerInput* input);

    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const PlayerInput* mInput = nullptr;
    bool mIsEnableDashInput = false;
    u8 _11[7] = {};
};

static_assert(sizeof(PlayerJudgeStartDash) == 0x18);
