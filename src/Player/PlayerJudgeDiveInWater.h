#pragma once

#include "Library/HostIO/HioNode.h"

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}
class PlayerConst;
class IUsePlayerHeightCheck;
class IUsePlayerFallDistanceCheck;

class PlayerJudgeDiveInWater : public al::HioNode, public IJudge {
public:
    PlayerJudgeDiveInWater(const al::LiveActor* player, const PlayerConst* playerConst,
                           const IUsePlayerHeightCheck* heightCheck,
                           const IUsePlayerFallDistanceCheck* fallDistanceCheck);

    void reset() override {}

    void update() override {}

    bool judge() const override;

private:
    const al::LiveActor* mPlayer;
    const PlayerConst* mConst;
    const IUsePlayerHeightCheck* mHeightCheck;
    const IUsePlayerFallDistanceCheck* mFallDistanceCheck;
};

static_assert(sizeof(PlayerJudgeDiveInWater) == 0x28);
