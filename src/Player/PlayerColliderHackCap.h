#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Player/IUsePlayerCollision.h"

namespace al {
class LiveActor;
}  // namespace al
class PlayerCollider;
class CollisionShapeKeeper;

// Minimal partial reconstruction of the cap's collision wrapper.
// The base class is inferred from the call site: HackCap::isSeparateHipDropLand
// passes its PlayerColliderHackCap* directly to rs::isJustLand, which takes a
// const IUsePlayerCollision*, and the original asm passes the pointer with no
// adjustment, so the base must sit at offset 0. Layout from
// PlayerColliderHackCap.o (ctor 0x435c44, init 0x435c60): +0x08 is the actor,
// +0x10 the PlayerCollider (returned by getPlayerCollider), +0x18 the
// CollisionShapeKeeper holding the "Body" sphere.
class PlayerColliderHackCap : public IUsePlayerCollision {
public:
    PlayerColliderHackCap(al::LiveActor* actor);

    void init();
    void updateCollider(const sead::Vector3f& pos);
    f32 getColliderRadius() const;
    PlayerCollider* getPlayerCollider() const override;

private:
    al::LiveActor* mActor;
    PlayerCollider* mPlayerCollider = nullptr;
    CollisionShapeKeeper* mShapeKeeper = nullptr;
};
