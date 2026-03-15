#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

class Chorobon : public al::LiveActor, public IUseDimension {
public:
    Chorobon(const char* name, const f32*);

    void init(const al::ActorInitInfo& info) override;
    void control() override;

    void exeMove();
    void exeReaction();

    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    ActorDimensionKeeper* getActorDimensionKeeper() const override;

private:
    char _110[0x160 - sizeof(al::LiveActor) - sizeof(IUseDimension)];
};
