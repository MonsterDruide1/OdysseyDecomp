#pragma once

#include "Library/LiveActor/LiveActor.h"

class Gotogoton : public al::LiveActor {
public:
    Gotogoton(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void makeFaceList(const sead::Vector3i&, f32);
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void endHack(bool);
    void control() override;

    void exeWait();
    void exeHackStart();
    void setNerveHackWait();
    void exeHackWait();
    void exeDontMoveReflect();
    void exeDontMove();
    void exeRotate();
    void exeRotateCancel();
    void exeHackEnd();
    void exeSuccess();
    void exeEnd();

private:
    char _108[0x1B8 - sizeof(al::LiveActor)];
};
