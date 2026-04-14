#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class MtxConnector;
class SensorMsg;
}  // namespace al

class ActorDimensionKeeper;
class BlockEmpty2D;
class BlockStateSingleItem;
class BlockStateTenCoin;

class BlockQuestion2D : public al::LiveActor, public IUseDimension {
public:
    BlockQuestion2D(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void appear() override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void endClipped() override;

    ActorDimensionKeeper* getActorDimensionKeeper() const override;

    void exeSingleItem();
    void exeTenCoin();

private:
    s32 mItemType = -1;
    BlockStateTenCoin* mTenCoinState = nullptr;
    BlockStateSingleItem* mSingleItemState = nullptr;
    BlockEmpty2D* mEmptyBlock2D = nullptr;
    ActorDimensionKeeper* mDimensionKeeper = nullptr;
    al::MtxConnector* mMtxConnector = nullptr;
    bool mIsConnectToCollisionBack = false;
};

static_assert(sizeof(BlockQuestion2D) == 0x148);
