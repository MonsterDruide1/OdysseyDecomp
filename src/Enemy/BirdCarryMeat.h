#pragma once

#include "Library/LiveActor/LiveActor.h"

class BirdCarryMeat : public al::LiveActor {
public:
    BirdCarryMeat(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;

    const sead::Matrix34f* getBindMtx() const;
    void skipDemo();
    void exeWaitOnRail();
    void exeDemoCarryMeat();
    void exeMoveMeat();
    void exeReactionCarry();
    void exeDrop();
    void exeFlyAway();
    bool isDropNerve() const;

    al::RailRider* getRailRider() const override;

private:
    char _108[0x180 - sizeof(al::LiveActor)];
};
