#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class CollisionPartsFilterActor;

class SurfMapParts : public LiveActor {
public:
    SurfMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
    void exeWait();

private:
    CollisionPartsFilterActor* mCollisionPartsFilterActor = nullptr;
    f32 mCheckOffset = 1000.0f;
    sead::Quatf mQuat = sead::Quatf::unit;
    sead::Vector3f mTrans = sead::Vector3f::zero;
    sead::Vector3f mUpDir = sead::Vector3f::ey;
    bool mIsEnableSlope = true;
};

static_assert(sizeof(SurfMapParts) == 0x140);
}  // namespace al
