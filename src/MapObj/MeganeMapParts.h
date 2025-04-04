#pragma once

#include "Library/LiveActor/LiveActor.h"

class MeganeMapParts : public al::LiveActor {
public:
    MeganeMapParts(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;

    void updateAlpha();
    void exeHide();
    void exeOn();
    void exeShow();
    void exeOff();
    void exeSwitchAppear();

private:
    f32 mAlpha = 0.0f;
};
