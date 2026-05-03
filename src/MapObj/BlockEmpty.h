#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class MtxConnector;
class SensorMsg;
}  // namespace al

class BlockEmpty : public al::LiveActor {
public:
    BlockEmpty(const char* name, const char* archiveName);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self) override;

    void setShadowDropLength(f32 shadowDropLength);
    void startReaction();
    void startReactionTransparent();
    void exeWait();
    void exeReaction();
    void exeReactionTransparent();

private:
    const char* mArchiveName = nullptr;
    bool mIsConnectToCollisionBack = false;
    al::MtxConnector* mMtxConnector = nullptr;
    sead::Vector3f mShadowDropClippingCenter = sead::Vector3f::zero;
};

static_assert(sizeof(BlockEmpty) == 0x130);
