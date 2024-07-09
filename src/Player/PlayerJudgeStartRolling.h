#pragma once

#include "Player/IJudge.h"

namespace al {
class LiveActor;
}
class PlayerConst;
class PlayerInput;
class IUsePlayerCollision;
class IPlayerModelChanger;
class PlayerCarryKeeper;

class PlayerJudgeStartRolling : public IJudge {
public:
    PlayerJudgeStartRolling(const al::LiveActor* player, const PlayerConst* pConst,
                            const PlayerInput* input, const IUsePlayerCollision* collider,
                            const IPlayerModelChanger* modelChanger,
                            const PlayerCarryKeeper* carryKeeper);
    bool isEnableTriggerRolling() const;
    bool judgeCancelHipDrop() const;
    bool isTriggerRestartSwing() const;

    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const al::LiveActor* mPlayer;
    const PlayerConst* mConst;
    const PlayerInput* mInput;
    const IUsePlayerCollision* mCollider;
    const IPlayerModelChanger* mModelChanger;
    const PlayerCarryKeeper* mCarryKeeper;
};

static_assert(sizeof(PlayerJudgeStartRolling) == 0x38);
