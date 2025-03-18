#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/Axis.h"

namespace al {
class SlideMapParts : public LiveActor {
public:
    SlideMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
    void start();
    bool receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) override;

    void exeStandBy();
    void exeDelay();
    void exeWait();
    void exeMove();

    s32 calcMoveTime() const;

private:
    sead::Matrix34f mSurfaceEffectMtx = sead::Matrix34f::ident;
    sead::Vector3f mTrans = sead::Vector3f::zero;
    Axis mMoveAxis = Axis::None;
    f32 mMoveDistance = 100.0f;
    f32 mMoveSpeed = 10.0f;
    s32 mWaitTime = 60;
    s32 mMoveTime = -1;
    s32 mDelayTime = 0;
    bool mIsMoveForwards = true;
};

static_assert(sizeof(SlideMapParts) == 0x160);
}  // namespace al
