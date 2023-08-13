#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveUtil.h"
#include "Util/IUseDimension.h"

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
