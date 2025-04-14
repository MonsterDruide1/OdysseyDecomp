#pragma once

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}
class PlayerAreaChecker;
class IPlayerModelChanger;

class PlayerJudgeStartRise : public IJudge {
public:
    PlayerJudgeStartRise(const al::LiveActor* player, const PlayerAreaChecker* areaChecker,
                         const IPlayerModelChanger* modelChanger);

    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const al::LiveActor* mPlayer;
    const PlayerAreaChecker* mAreaChecker;
    const IPlayerModelChanger* mModelChanger;
};

static_assert(sizeof(PlayerJudgeStartRise) == 0x20);
