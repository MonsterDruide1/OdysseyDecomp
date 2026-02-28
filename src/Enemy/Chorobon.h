#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

class Chorobon : public al::LiveActor, public IUseDimension {
public:
    Chorobon(const char* name, const f32*);

    void init(const al::ActorInitInfo&) override;
    void control() override;

    void exeMove();
    void exeReaction();

    void attackSensor(al::HitSensor*, al::HitSensor*) override;
    bool receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*) override;
    ActorDimensionKeeper* getActorDimensionKeeper() const override;

private:
    char filler_110[0x50];
};

static_assert(sizeof(Chorobon) == 0x160);
