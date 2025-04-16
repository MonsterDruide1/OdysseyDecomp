#pragma once

#include <math/seadQuat.h>

#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/Axis.h"

namespace al {
class RotateMapParts : public LiveActor {
public:
    RotateMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
    void start();
    bool receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) override;
    void appearAndSetStart();

    void exeStandBy();
    void exeRotate();
    void exeAssistStop();

private:
    sead::Quatf mQuat = sead::Quatf::unit;
    Axis mRotateAxis = Axis::X;
    f32 mRotateSpeed = 100.0f;
    s32 mAssistTimer = 0;
};

static_assert(sizeof(RotateMapParts) == 0x128);
}  // namespace al
