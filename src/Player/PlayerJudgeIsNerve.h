#pragma once

#include "Player/IJudge.h"

namespace al {
class IUseNerve;
class Nerve;
}  // namespace al

class PlayerJudgeIsNerve : public IJudge {
public:
    PlayerJudgeIsNerve(const al::IUseNerve* nerveUser, const al::Nerve* nerve);

    bool judge() const override;

    void reset() override {}

    void update() override {}

private:
    const al::IUseNerve* mNerveUser;
    const al::Nerve* mNerve;
};
