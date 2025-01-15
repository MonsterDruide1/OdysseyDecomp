#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

enum class FurnitureType : u64 {
    Chair = 0,
    Bed = 1,
};

enum class PlayerFurnitureState : u32 {
    AirFacing = 0,
    AirNotFacing = 1,
    Bound = 2,
    Unbound = 3,
};

class FurnitureStateWait : public al::ActorStateBase {
public:
    FurnitureStateWait(al::LiveActor* actor, FurnitureType furniture);
    void appear() override;
    bool isEnableBindRequest() const;
    void exeWait();
    void exeBindRequest();

private:
    FurnitureType mFurnitureType : 32;
    PlayerFurnitureState mPlayerState = PlayerFurnitureState::Unbound;
    bool mIsPlayerNotOnGround = false;
    sead::Vector3f mPlayerPos = sead::Vector3f::zero;
};
