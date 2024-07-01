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

class PlayerJudgeGrabCeil : public al::HioNode, public IJudge {
public:
    PlayerJudgeGrabCeil(const al::LiveActor* player, const PlayerConst* pConst,
                        const IUsePlayerCollision* collider,
                        const IPlayerModelChanger* modelChanger,
                        const PlayerCarryKeeper* carryKeeper,
                        const PlayerExternalVelocity* externalVelocity);
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
    bool mIsJudge = false;
    const al::CollisionParts* mCollidedParts = nullptr;
    sead::Vector3f _48 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _54 = {0.0f, 0.0f, 0.0f};
    sead::Vector3f _60 = {0.0f, 0.0f, 0.0f};
};
