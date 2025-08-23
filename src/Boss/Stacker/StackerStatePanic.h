#pragma once

#include "Library/Nerve/NerveStateBase.h"
#include "Project/HitSensor/HitSensor.h"

namespace al {
class SensorMsg;
}  // namespace al
class Stacker;

class StackerStatePanic : public al::ActorStateBase {
public:
    StackerStatePanic(Stacker*);
    void appear() override;
    void kill() override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other);
    bool receiveMsg(const al::SensorMsg* self, al::HitSensor* other, al::HitSensor* hitSensor);
    bool receiveMsgDamage(const al::SensorMsg*, al::HitSensor*, al::HitSensor*);
    void setIsRunAway(bool);
    void exeSurprise();
    void exeWait();
    void exeRunStart();
    void exeRun();
    void exeTurn();
    void exeCapHit();
    ~StackerStatePanic() override;

private:
    bool mIsRunAway = false;
    sead::Vector3f _24 = sead::Vector3f::zero;
    s32 _30 = 0;
    s32 _34 = 0;
    s8 _38 = 0;
    f32 _3c = 1.0;
};
