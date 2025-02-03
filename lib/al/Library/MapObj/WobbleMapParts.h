#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class WobbleMapParts : public LiveActor {
public:
    WobbleMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
    bool receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) override;

    void appearAndSetStart();
    void exeWait();
    void updateMove();
    void exeMove();
    void exeAssistStop();

private:
    sead::Quatf _108 = sead::Quatf::unit;
    sead::Vector3f _118 = sead::Vector3f::ey;
    sead::Quatf _124 = sead::Quatf::unit;
    sead::Vector3f _134 = sead::Vector3f::zero;
    sead::Vector3f _140 = sead::Vector3f::ey;
    sead::Vector3f _14c = sead::Vector3f::ey;
    f32 mMaxRotate = 10.0f;
    f32 _15c = 0.0f;
    s32 mAssistStopTimer = 0;
    bool mIsStop = false;
};

static_assert(sizeof(WobbleMapParts) == 0x168);
}  // namespace al
