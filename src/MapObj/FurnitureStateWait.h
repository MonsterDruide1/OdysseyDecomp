#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

enum FurnitureType : u64 {
    Chair = 0,
    Bed = 1,
};

class FurnitureStateWait : public al::ActorStateBase {
public:
    FurnitureStateWait(al::LiveActor* actor, FurnitureType furniture);
    void appear() override;
    bool isEnableBindRequest() const;
    void exeWait();
    void exeBindRequest();

private:
    u32 mFurnitureType;
    u32 _24 = 3;
    bool mIsPlayerNotOnGround = false;
    sead::Vector3f mPlayerPos = sead::Vector3f::zero;
};
