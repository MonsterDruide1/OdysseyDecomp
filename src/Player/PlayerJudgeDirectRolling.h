#pragma once

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}  // namespace al
class PlayerConst;
class PlayerInput;
class IUsePlayerCollision;
class IPlayerModelChanger;
class PlayerCarryKeeper;
class PlayerCounterForceRun;

class PlayerJudgeDirectRolling : public IJudge {
public:
    PlayerJudgeDirectRolling(const al::LiveActor* player, const PlayerConst* pConst,
                             const PlayerInput* input, const IUsePlayerCollision* collider,
                             const IPlayerModelChanger* modelChanger,
                             const PlayerCarryKeeper* carryKeeper,
                             const PlayerCounterForceRun* counterForceRun);

    void reset() override;
    void update() override;

    bool judge() const override { return mIsJudge; }

private:
    const al::LiveActor* mPlayer;
    const PlayerConst* mConst;
    const PlayerInput* mInput;
    const IUsePlayerCollision* mCollider;
    const IPlayerModelChanger* mModelChanger;
    const PlayerCarryKeeper* mCarryKeeper;
    const PlayerCounterForceRun* mCounterForceRun;
    bool mIsJudge = false;
};
