#pragma once

#include "Library/Nerve/NerveStateBase.h"
#include "Project/HitSensor/HitSensor.h"

namespace al {
class SensorMsg;
}  // namespace al
class Stacker;

class StackerStatePanic : public al::ActorStateBase {
public:
    StackerStatePanic(Stacker* stacker);
    void appear() override;
    void kill() override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other);
    bool receiveMsg(const al::SensorMsg* self, al::HitSensor* other, al::HitSensor* hitSensor);
    bool receiveMsgDamage(const al::SensorMsg* self, al::HitSensor* other,
                          al::HitSensor* _hitSensor);
    void setIsRunAway(bool isRunAway);
    void exeSurprise();
    void exeWait();
    void exeRunStart();
    void exeRun();
    void exeTurn();
    void exeCapHit();

private:
    bool mIsRunAway = false;
    sead::Vector3f mTurnTarget = sead::Vector3f::zero;
    s32 mInvincibleCounter = 0;
    s32 mPanicCounter = 0;
    bool mIsKillByTimer = false;
    f32 mScale = 1.0f;
};
