#pragma once

#include "Library/LiveActor/LiveActor.h"

class Jango : public al::LiveActor {
public:
    Jango(const char* name);

    void init(const al::ActorInitInfo&) override;
    void initAfterPlacement() override;
    void attackSensor(al::HitSensor*, al::HitSensor*) override;
    bool receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*) override;
    void control() override;

    bool isEnableCap() const;
    bool tryOffCapCatchedSwitch();
    void exeWaitCapGetDemo();
    void exeCapGetDemo();
    void exeEscape();
    void exeSurprise();
    void exeBlowDown();

private:
    char _108[0x188 - sizeof(al::LiveActor)];
};

class JangoFunction {
public:
    void setCapTransOnJoint(al::LiveActor*, al::LiveActor*);
    void resetCapTransOnJoint(al::LiveActor*, al::LiveActor*);
    void resetTransJointRoot(al::LiveActor*);
};
