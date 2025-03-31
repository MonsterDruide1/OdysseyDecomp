#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class TimeBalloonHintArrow : public al::LiveActor {
public:
    TimeBalloonHintArrow();

    void init(const al::ActorInitInfo& initInfo) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void appear() override;

    void updateTrans();
    void updateRotate(const sead::Vector3f&);
    void startAlphaNerve();

    void exeWait();
    void exeWaitBeforeAlpha();
    void exeAlpha();
};
