#pragma once

#include "Library/LiveActor/LiveActor.h"

// Unused class
class CornBoy : public al::LiveActor {
public:
    CornBoy(const char* name);

    void init(const al::ActorInitInfo&) override;
    void control() override;
    void attackSensor(al::HitSensor*, al::HitSensor*) override;
    bool receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*) override;

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
