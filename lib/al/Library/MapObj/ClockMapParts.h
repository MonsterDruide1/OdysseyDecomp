#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class RippleCtrl;

class ClockMapParts : public LiveActor {
public:
    ClockMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
    void start();
    void standBy();
    bool receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) override;
    void appearAndSetStart();
    void setRestartNerve();
    void control() override;
    void setRotateStartNerve();

    void exeStandBy();
    void exeDelay();
    void exeRotateSign();
    void exeRotate();
    void exeWait();
    void exeAssistStop();
    void exeAssistStopSync();
    void exeAssistStopEndWait();

private:
    sead::Quatf mQuat = sead::Quatf::unit;
    s32 mRotateAxis = 0;
    s32 mClockAngleDegree = 0;
    s32 _120 = 0;
    s32 _124 = 1;
    s32 mDelayTime = 0;
    s32 mRotateTime = 30;
    s32 mWaitTime = 30;
    s32 mRotateSignTime = 0;
    s32 _138 = 30;
    s32 _13c;
    s32 _140 = 0;
    s32 _144;
    RippleCtrl* mRippleCtrl = nullptr;
};

static_assert(sizeof(ClockMapParts) == 0x150);
}  // namespace al
