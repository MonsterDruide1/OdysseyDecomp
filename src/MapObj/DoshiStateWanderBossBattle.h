#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class Doshi;

class DoshiStateWanderBossBattle : public al::HostStateBase<Doshi> {
public:
    DoshiStateWanderBossBattle(Doshi* doshi);

    void appear() override;

    virtual bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                            al::HitSensor* self);
    void setting(const sead::Vector3f& position, const sead::Vector3f& front);

    void exeWait();
    void exeReaction();

private:
    sead::Vector3f mPosition = sead::Vector3f::zero;
    sead::Vector3f mFront = sead::Vector3f::ez;
};
