#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
class PlacementId;
}  // namespace al

class CoinStack;

class CoinStackGroup : public al::LiveActor {
public:
    CoinStackGroup(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    void control() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void makeActorDead() override;
    void makeActorAlive() override;

    void generateCoinStackGroup(const al::ActorInitInfo&, s32);
    void makeStackAppear();
    void makeStackDisappear();
    f32 setStackAsCollected(CoinStack* stack);
    f32 updateClippingInfo(u32 stackAmount);
    void validateClipping();

private:
    sead::Vector3f mClippingPos = sead::Vector3f(0.0f, 0.0f, 0.0f);
    s32 mStackAmount = 0;
    CoinStack* mCoinStack = nullptr;
    al::PlacementId* mPlacementId = nullptr;
    bool mIsMustSave = true;
};
