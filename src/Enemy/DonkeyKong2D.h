#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

class DonkeyKong2D : public al::LiveActor, IUseDimension {
public:
    DonkeyKong2D(const char* name);
    void init(const al::ActorInitInfo&) override;
    void attackSensor(al::HitSensor*, al::HitSensor*) override;
    bool receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*) override;

    void exeWait();
    void exeThrow();
    void exeDamage();
    void exeDown();

    ActorDimensionKeeper* getActorDimensionKeeper() const override;

private:
    char _108[0x140 - sizeof(al::LiveActor) - sizeof(IUseDimension)];
};
