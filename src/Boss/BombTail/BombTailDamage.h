#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class BombTailDamage : public al::LiveActor {
public:
    BombTailDamage();

    void init(const al::ActorInitInfo& info) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void startAppear(const sead::Vector3f& trans, s32 delayStep, bool isChance);
    void forceDisappear();

    void exeBeforeBattle();
    void exeDelay();
    void exeAppear();
    void exeWait();
    void exeDisappear();
    void exeAppearChance();
    void exeWaitChance();
    void exeDisappearChance();

private:
    s32 mDelayStep = 0;
    bool mIsChance = false;
};

static_assert(sizeof(BombTailDamage) == 0x110);
