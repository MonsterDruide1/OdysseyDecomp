#pragma once

#include "Player/IJudge.h"

namespace al {
class IUseNerve;
class Nerve;
}  // namespace al

class PlayerJudgeSameNerve : public IJudge {
public:
    PlayerJudgeSameNerve(const al::IUseNerve* nerveUser, const al::Nerve* nerve);
    void reset();
    void update();
    bool judge() const;

private:
    const al::IUseNerve* mNerveUser;
    const al::Nerve* mNerve;
};

static_assert(sizeof(PlayerJudgeSameNerve) == 0x18);
