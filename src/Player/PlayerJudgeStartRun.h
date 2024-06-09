#pragma once

#include <basis/seadTypes.h>

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}
class IUsePlayerCollision;
class PlayerConst;
class PlayerInput;
class PlayerCounterForceRun;

class PlayerJudgeStartRun : public IJudge {
public:
    PlayerJudgeStartRun(const al::LiveActor* player, const PlayerConst* pConst,
                              const IUsePlayerCollision* collision, const PlayerInput* input,
                              const PlayerCounterForceRun* counterForceRun,
                              const IJudge* judgeWaterSurfaceRun);

    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const al::LiveActor* mPlayer;
    const IUsePlayerCollision* mCollision;
    const PlayerInput* mInput;
    const PlayerCounterForceRun* mCounterForceRun;
    const IJudge* mJudgeForceLand;
};
