#pragma once

#include <math/seadVector.h>

#include "Library/HostIO/HioNode.h"

#include "Player/IJudge.h"

namespace al {
class LiveActor;
class CollisionParts;
}  // namespace al
class PlayerConst;
class IUsePlayerCollision;
class IPlayerModelChanger;
class PlayerCarryKeeper;
class PlayerExternalVelocity;
class PlayerInput;
class PlayerTrigger;
class PlayerCounterForceRun;

class PlayerJudgeWallCatch : public al::HioNode, public IJudge {
public:
    PlayerJudgeWallCatch(const al::LiveActor* player, const PlayerConst* pConst,
                         const IUsePlayerCollision* collider,
                         const IPlayerModelChanger* modelChanger,
                         const PlayerCarryKeeper* carryKeeper,
                         const PlayerExternalVelocity* externalVelocity, const PlayerInput* input,
                         const PlayerTrigger* trigger,
                         const PlayerCounterForceRun* counterForceRun);

    void reset() override;
    void update() override;
    bool judge() const override;

public:
    const al::LiveActor* mPlayer;
    const PlayerConst* mConst;
    const IUsePlayerCollision* mCollision;
    const IPlayerModelChanger* mModelChanger;
    const PlayerCarryKeeper* mCarryKeeper;
    const PlayerExternalVelocity* mExternalVelocity;
    const PlayerInput* mInput;
    const PlayerTrigger* mTrigger;
    const PlayerCounterForceRun* mCounterForceRun;
    bool mIsJudge = false;
    const al::CollisionParts* mCollidedWallPart = nullptr;
    sead::Vector3f mPosition = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mCollidedWallNormal = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mNormalAtPos = {0.0f, 0.0f, 0.0f};
};
static_assert(sizeof(PlayerJudgeWallCatch) == 0x88);
