#pragma once

#include "Library/HostIO/HioNode.h"

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}
class PlayerConst;
class PlayerInput;
class IUsePlayerCollision;
class IPlayerModelChanger;
class IUsePlayerHeightCheck;
class PlayerCounterAfterUpperPunch;
class PlayerWallActionHistory;
class PlayerCarryKeeper;
class PlayerTrigger;
class PlayerCounterForceRun;

class PlayerJudgeWallKeep : public al::HioNode, public IJudge {
public:
    PlayerJudgeWallKeep(const al::LiveActor* player, const PlayerConst* pConst,
                        const PlayerInput* input, const IUsePlayerCollision* collider,
                        const IPlayerModelChanger* modelChanger,
                        const IUsePlayerHeightCheck* heightCheck,
                        const PlayerCounterAfterUpperPunch* counterAfterUpperPunch,
                        const PlayerWallActionHistory* wallActionHistory,
                        const PlayerCarryKeeper* carryKeeper, const PlayerTrigger* trigger,
                        const PlayerCounterForceRun* counterForceRun);

    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const al::LiveActor* mPlayer;
    const PlayerConst* mConst;
    const PlayerInput* mInput;
    const IUsePlayerCollision* mCollider;
    const IPlayerModelChanger* mModelChanger;
    const IUsePlayerHeightCheck* mHeightCheck;
    const PlayerCounterAfterUpperPunch* mCounterAfterUpperPunch;
    const PlayerWallActionHistory* mWallActionHistory;
    const PlayerCarryKeeper* mCarryKeeper;
    const PlayerTrigger* mTrigger;
    const PlayerCounterForceRun* mCounterForceRun;
    bool mIsJudge = false;
};
