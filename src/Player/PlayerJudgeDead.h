#pragma once

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}

class PlayerJudgeDead : public IJudge {
public:
    PlayerJudgeDead(const al::LiveActor* player);

    void reset() override {}

    void update() override {}

    bool judge() const override;

private:
    const al::LiveActor* mPlayer;
};
