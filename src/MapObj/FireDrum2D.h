#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/IUseDimension.h"

namespace al {
struct ActorInitInfo;
}
class ActorDimensionKeeper;

class FireDrum2D : public al::LiveActor, public IUseDimension {
public:
    FireDrum2D(const char* name);
    void init(const al::ActorInitInfo& info) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeWait();
    void exeBurn();

    ActorDimensionKeeper* getActorDimensionKeeper() const override;

private:
    ActorDimensionKeeper* mActorDimensionKeeper = nullptr;
};
