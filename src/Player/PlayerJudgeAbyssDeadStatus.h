#pragma once

#include "Player/IJudge.h"
#include "Player/PlayerJudgeSameNerve.h"

class PlayerJudgeSameNerve;
class PlayerRecoverySafetyPoint;

class PlayerJudgeAbyssDeadStatus : public IJudge {
public:
    PlayerJudgeAbyssDeadStatus(const PlayerJudgeSameNerve* nerve,
                               const PlayerRecoverySafetyPoint* safePoint);

    virtual void reset() override;
    virtual void update() override;
    virtual bool judge() const override;

private:
    const PlayerJudgeSameNerve* mNerve;
    const PlayerRecoverySafetyPoint* mSafePoint;
};
