#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class ActorInitInfo;
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
    void updateClippingInfo(u32);
    void validateClipping();

private:
    sead::Vector3f _108;
    s32 _114;
    CoinStack* _118;
    al::PlacementId* _120;
    bool _128;
};
