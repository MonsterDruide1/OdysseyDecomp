#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

class Bubble2D : public al::LiveActor, public IUseDimension {
public:
    Bubble2D(const char* name);

    void init(const al::ActorInitInfo&) override;
    void attackSensor(al::HitSensor*, al::HitSensor*) override;
    void exeInitDelay();

    void exeWait();
    void exeMove();
    void exeReturn();

    ActorDimensionKeeper* getActorDimensionKeeper() const override;

private:
    char _108[0x148 - sizeof(al::LiveActor) - sizeof(IUseDimension)];
};
