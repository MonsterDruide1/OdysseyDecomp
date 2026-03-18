#pragma once

#include "Player/IJudge.h"

class PlayerJudgeSameNerve;
class PlayerRecoverySafetyPoint;

class PlayerJudgeAbyssDeadStatus : public IJudge {
public:
    PlayerJudgeAbyssDeadStatus(const PlayerJudgeSameNerve* nerve,
                               const PlayerRecoverySafetyPoint* safePoint);

    bool judge() const override;
    void reset() override;
    void update() override;

private:
    const PlayerJudgeSameNerve* mNerve;
    const PlayerRecoverySafetyPoint* mSafePoint;
};

static_assert(sizeof(PlayerJudgeAbyssDeadStatus) == 0x18);
