#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

class DonkeyKong2D : public al::LiveActor, public IUseDimension {
public:
    DonkeyKong2D(const char* name);
    void init(const al::ActorInitInfo& info) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeWait();
    void exeThrow();
    void exeDamage();
    void exeDown();

    ActorDimensionKeeper* getActorDimensionKeeper() const override;

private:
    char _110[0x140 - sizeof(al::LiveActor) - sizeof(IUseDimension)];
};
