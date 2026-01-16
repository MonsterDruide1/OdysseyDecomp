#pragma once

#include <container/seadPtrArray.h>

#include "Player/IJudge.h"

class PlayerJudgeOr : public IJudge {
public:
    PlayerJudgeOr(s32 maxJudges);

    void addJudge(IJudge* judge);
    bool judge() const override;

    void reset() override {}

    void update() override {}

private:
    sead::PtrArray<IJudge> mJudges;
};
