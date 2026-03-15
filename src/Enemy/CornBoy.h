#pragma once

#include "Library/LiveActor/LiveActor.h"

// Unused class
class CornBoy : public al::LiveActor {
public:
    CornBoy(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void endCapture();
    void exeWait();
    void exeSwoon();
    void exeCaptureStart();
    void exeCaptureStartEnd();
    void exeCaptureEnd();
    void isEnableCharge() const;
    bool tryCharge();
    bool tryStartLaunch();
    void exeCaptureWait();
    void exeCaptureRun();
    void exeCaptureAim();
    void exeCaptureLaunchStart();
    void exeCaptureLaunch();
    void exeCaptureLaunchEnd();
    void exeCaptureFall();
    void exeCaptureLand();
    void exeCaptureCoolDown();
    void exeDie();
};
