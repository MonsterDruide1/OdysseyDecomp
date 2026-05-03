#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class MtxConnector;
class SensorMsg;
}  // namespace al

class ActorDimensionKeeper;

class BlockBrickBig2D : public al::LiveActor, public IUseDimension {
public:
    BlockBrickBig2D(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void startBreak();
    void initAfterPlacement() override;
    void kill() override;
    bool receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self) override;
    void exeWait();

    ActorDimensionKeeper* getActorDimensionKeeper() const override { return mDimensionKeeper; }

private:
    ActorDimensionKeeper* mDimensionKeeper = nullptr;
    al::MtxConnector* mMtxConnector = nullptr;
};

static_assert(sizeof(BlockBrickBig2D) == 0x120);
