#pragma once

#include "al/LiveActor/LiveActor.h"
#include "al/util/NerveUtil.h"
#include "game/iuse/IUseDimension.h"

namespace al {
class ActorInitInfo;
}
class ActorDimensionKeeper;

class FireDrum2D : public al::LiveActor, public IUseDimension {
public:
    FireDrum2D(const char* name);
    void init(const al::ActorInitInfo& info) override;
    void attackSensor(al::HitSensor* target, al::HitSensor* source) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* source,
                    al::HitSensor* target) override;

    void exeWait();
    void exeBurn();

    ActorDimensionKeeper* getActorDimensionKeeper() const override;

private:
    ActorDimensionKeeper* mActorDimensionKeeper = nullptr;
};

namespace {
NERVE_HEADER(FireDrum2D, Wait)
NERVE_HEADER(FireDrum2D, Burn)
}  // namespace
