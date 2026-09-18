#pragma once

#include <math/seadVector.h>

#include "Player/HackerStateBase.h"

class IUsePlayerCollision;
class IUsePlayerHack;
class PlayerAnimator;
class PlayerConst;

namespace al {
class CollisionParts;
class CollisionPartsConnector;
class LiveActor;
}  // namespace al

class YoshiStateHackWallCling : public HackerStateBase {
public:
    YoshiStateHackWallCling(al::LiveActor* actor, IUsePlayerHack** hacker,
                            const PlayerConst* pConst, const IUsePlayerCollision* collision,
                            PlayerAnimator* animator);

    void appear() override;
    void setup(const al::CollisionParts* collisionParts, const sead::Vector3f& position,
               const sead::Vector3f& normal);
    void exeCling();

private:
    const PlayerConst* mConst;
    const IUsePlayerCollision* mCollision;
    PlayerAnimator* mAnimator;
    const al::CollisionParts* mCollisionParts;
    sead::Vector3f mPosition;
    sead::Vector3f mNormal;
    al::CollisionPartsConnector* mCollisionPartsConnector;
};

static_assert(sizeof(YoshiStateHackWallCling) == 0x68);
