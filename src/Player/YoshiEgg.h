#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class IUsePlayerCollision;
class FukankunZoomCapMessage;

class YoshiEgg : public al::LiveActor {
public:
    YoshiEgg(const al::LiveActor* parent, const IUsePlayerCollision* playerCollision);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void initPlacementEgg();
    void appearEgg();
    bool isEndAppear() const;
    bool isBreak() const;

    void exeAppear();
    void exeWait();
    void exeBreak();

private:
    const al::LiveActor* mParent;
    const IUsePlayerCollision* mPlayerCollision;
    FukankunZoomCapMessage* mFukankunZoomCapMessage;
};

static_assert(sizeof(YoshiEgg) == 0x120);
