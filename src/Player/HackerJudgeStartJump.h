#pragma once

#include "Player/HackerJudge.h"

class IUsePlayerHack;

class HackerJudgeStartJump : public HackerJudge {
public:
    HackerJudgeStartJump(IUsePlayerHack**);
    void reset() override;
    void update() override;
    bool judge() const override;
};
