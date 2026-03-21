#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class JokuHand : public al::LiveActor {
public:
    JokuHand(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self) override;
    void control() override;
    void appear() override;

    void damage();

    void exeWait();
    void exeDamage();

private:
    const sead::Matrix34f* mMtx = nullptr;
};
