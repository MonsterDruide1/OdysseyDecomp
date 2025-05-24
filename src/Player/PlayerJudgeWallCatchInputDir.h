#pragma once

#include <math/seadVector.h>

#include "Library/HostIO/HioNode.h"

#include "Player/IJudge.h"

namespace al {
class LiveActor;
class CollisionParts;
class CollisionPartsConnector;
class CollisionPartsKeeper;
}  // namespace al
class PlayerConst;
class PlayerInput;
class IUsePlayerCollision;
class IPlayerModelChanger;
class PlayerCarryKeeper;
class PlayerExternalVelocity;
class PlayerCounterForceRun;
class IUsePlayerHeightCheck;

class PlayerJudgeWallCatchInputDir : public al::HioNode, public IJudge {
public:
    PlayerJudgeWallCatchInputDir(const al::LiveActor* player, const PlayerConst* pConst,
                                 const PlayerInput* input, const IUsePlayerCollision* collision,
                                 const IPlayerModelChanger* modelChanger,
                                 const PlayerCarryKeeper* carryKeeper,
                                 const PlayerExternalVelocity* externalVelocity,
                                 const PlayerCounterForceRun* counterForceRun,
                                 const IUsePlayerHeightCheck* heightCheck);
    void reset() override;
    void update() override;
    bool updateJudgeKids();
    bool updateJudgeNormal();
    bool judge() const override;
    void validateFallJudge();
    void updateWallCatchEnviroment();

private:
    const al::LiveActor* mPlayer;
    const PlayerConst* mConst;
    const PlayerInput* mInput;
    const IUsePlayerCollision* mCollision;
    const IPlayerModelChanger* mModelChanger;
    const PlayerCarryKeeper* mCarryKeeper;
    const PlayerExternalVelocity* mExternalVelocity;
    const PlayerCounterForceRun* mCounterForceRun;
    const IUsePlayerHeightCheck* mHeightCheck;
    bool mIsJudge = false;
    const al::CollisionParts* mCollisionParts = nullptr;
    sead::Vector3f mHitPos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mHitNormal2 = {0.0f, 0.0f, 0.0f};  // TODO find proper name for this one
    sead::Vector3f mHitNormal = {0.0f, 0.0f, 0.0f};
    s32 mValidTimer = 0;
    bool mIsOnGroundSkateCode = false;
    bool mIsKidsMode = false;
    al::CollisionPartsConnector* mCollisionPartsConnector = nullptr;
    sead::Vector3f mGroundNormal = {0.0f, 0.0f, 0.0f};
    s32 mTimeInAir = 0;
};

static_assert(sizeof(PlayerJudgeWallCatchInputDir) == 0xA8);
