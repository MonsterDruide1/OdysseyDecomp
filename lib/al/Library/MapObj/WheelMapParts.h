#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class SwitchKeepOnAreaGroup;
class SwitchOnAreaGroup;
class WheelMovement;

class WheelMapParts : public LiveActor {
public:
    WheelMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
    void control() override;
    bool receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) override;

    void appearAndSetStart();

    void exeWait();
    void exeMove();
    void exeAssistStop();

private:
    sead::Matrix34f mSurfaceEffectMtx = sead::Matrix34f::ident;
    WheelMovement* mWheelMovement = nullptr;
    SwitchKeepOnAreaGroup* mSwitchKeepOnAreaGroup = nullptr;
    SwitchOnAreaGroup* mSwitchOnAreaGroup = nullptr;
    s32 mAssistStopTimer = 0;
};

static_assert(sizeof(WheelMapParts) == 0x158);
}  // namespace al
