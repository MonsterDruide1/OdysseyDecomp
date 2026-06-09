#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

#include "Util/ItemUtil.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class MtxConnector;
class SensorMsg;
}  // namespace al
class BlockEmpty;
class BlockStateSingleItem;
class BlockStateTenCoin;

class BlockQuestion : public al::LiveActor {
public:
    BlockQuestion(const char* name);
    void init(const al::ActorInitInfo& initInfo) override;
    void listenApeear();
    void listenKill();
    void initAfterPlacement() override;
    void appear() override;
    void control() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    void exeSingleItem();
    void exeTenCoin();
    void exeDead();

private:
    rs::ItemType::ValueType mItemType = rs::ItemType::ValueType::None;
    BlockStateTenCoin* mBlockStateTenCoin = nullptr;
    BlockStateSingleItem* mBlockStateSingleItem = nullptr;
    BlockEmpty* mBlockEmpty = nullptr;
    al::MtxConnector* mMtxConnector = nullptr;
    bool mIsConnectToCollisionBack = false;
    sead::Vector3f _134 = sead::Vector3f::zero;
    f32 mShadowLength = 0.0f;
};

static_assert(sizeof(BlockQuestion) == 0x148);
