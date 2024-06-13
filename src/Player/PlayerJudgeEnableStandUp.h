#pragma once

#include "Player/IJudge.h"

class IUsePlayerCeilingCheck;
class IUsePlayerCollision;

class PlayerJudgeEnableStandUp : public IJudge {
public:
    PlayerJudgeEnableStandUp(const IUsePlayerCollision* collider,
                             const IUsePlayerCeilingCheck* ceilingChecker);

    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const IUsePlayerCollision* mCollider;
    const IUsePlayerCeilingCheck* mCeilingChecker;
};
