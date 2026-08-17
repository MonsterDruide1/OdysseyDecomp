#pragma once

#include "Library/LiveActor/LiveActor.h"

class IUsePlayerCollision;
class FukankunZoomCapMessage;

class YoshiEgg : public al::LiveActor {
public:
    YoshiEgg(const al::LiveActor* actor, const IUsePlayerCollision* collisionUser);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void initPlacementEgg();
    void appearEgg();

    bool isBreak() const;
    bool isEndAppear() const;

    void exeWait();
    void exeAppear();
    void exeBreak();

private:
    const al::LiveActor* mYoshiActor;
    const IUsePlayerCollision* mCollisionUser;
    FukankunZoomCapMessage* mZoomCapMessage;
};

static_assert(sizeof(YoshiEgg) == 0x120);
