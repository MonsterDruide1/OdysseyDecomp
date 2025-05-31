#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

class ActorDimensionKeeper;

class BarrelStack2D : public al::LiveActor, public IUseDimension {
public:
    BarrelStack2D(const char* name);
    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void doBreak();
    void exeWait();
    void exeBreak();

    ActorDimensionKeeper* getActorDimensionKeeper() const override { return mDimensionKeeper; }

private:
    ActorDimensionKeeper* mDimensionKeeper = nullptr;
};

static_assert(sizeof(BarrelStack2D) == 0x118);
