#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

class ActorDimensionKeeper;

namespace al {
class MtxConnector;
}  // namespace al

class BlockEmpty2D : public al::LiveActor, public IUseDimension {
public:
    BlockEmpty2D(const char* name);
    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void control() override;
    void appear() override;
    bool receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self) override;
    ActorDimensionKeeper* getActorDimensionKeeper() const override;

    void setMtxConnector(al::MtxConnector* mtxConnector) { mMtxConnector = mtxConnector; }

private:
    bool mIsConnectToCollisionBack = false;
    al::MtxConnector* mMtxConnector = nullptr;
    ActorDimensionKeeper* mDimensionKeeper = nullptr;
};

static_assert(sizeof(BlockEmpty2D) == 0x128);
