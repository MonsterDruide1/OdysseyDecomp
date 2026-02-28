#pragma once

#include "Library/LiveActor/LiveActor.h"

class Imomu : public al::LiveActor {
public:
    Imomu(const char* name);

    void init(const al::ActorInitInfo&) override;
    void control() override;
    bool tryResetHack(bool);
    void updateCameraTarget();
    void updateCollider() override;
    void attackSensor(al::HitSensor*, al::HitSensor*) override;
    bool receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*) override;

    void endHack(bool);
    void exeEnemy();
    void exeHackMove();
    void exeHackExpandSign();
    bool tryEndHackExpand(bool);
    void exeHackExpand();
    void updateExpandCore(bool*, bool*, bool*, f32);
    void exeHackExpandWait();
    void exeHackExpandHold();
    void exeHackEndExpandNoShrink();
    void exeHackShrinkToBody();
    void exeShrinkToHead();
    void exeHackShrinkReaction();
    u32 getVisibleBodyNum() const;

private:
    char _108[0x1C0 - sizeof(al::LiveActor)];
};
