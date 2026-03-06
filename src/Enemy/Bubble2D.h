#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

class Bubble2D : public al::LiveActor, public IUseDimension {
public:
    Bubble2D(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    void exeInitDelay();

    void exeWait();
    void exeMove();
    void exeReturn();

    ActorDimensionKeeper* getActorDimensionKeeper() const override;

private:
    char _110[0x148 - sizeof(al::LiveActor) - sizeof(IUseDimension)];
};
