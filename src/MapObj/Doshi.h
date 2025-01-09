#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class Doshi : public al::LiveActor {
public:
    Doshi(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;

    void onBattleStart();
    void doReaction(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);
    void requestUpdateAction(f32, f32);
    void isInWater() const;
    void requestStartCamera();
    void requestEndCamera();

    void exeWait();
    void exeRun();
    void exeReaction();
    void exeBossBattle();
};
