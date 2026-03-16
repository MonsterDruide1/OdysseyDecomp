#pragma once

#include "Util/HackerJudge.h"

class IUsePlayerHack;

class HackerJudgeStartJump : public HackerJudge {
public:
    HackerJudgeStartJump(IUsePlayerHack**);
    void reset() override;
    void update() override;
    bool judge() const override;
};

static_assert(sizeof(HackerJudgeStartJump) == 0x10);
