#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class GamaneBullet : public al::LiveActor {
public:
    GamaneBullet(const char* name, const al::LiveActor* parent);
    void init(const al::ActorInitInfo& info) override;
    void attackSensor(al::HitSensor* target, al::HitSensor* source) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* source,
                    al::HitSensor* target) override;
    void appear() override;
    void control() override;

    void shoot(f32, const sead::Vector3f&);
    void rotate(f32);

    void exeStraight();
    void exeBrake();
    void exeFall();
    void exeHit();

    al::LiveActor* getParent() { return mParentActor; }

private:
    f32 _108;
    al::LiveActor* mParentActor;
};
