#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

class EnemyStateWander;
class ExternalForceKeeper;

class Togezo : public al::LiveActor {
public:
    Togezo(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void listenAppear();
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    void control() override;
    void updateCollider() override;

    void exeWait();
    void exeWander();
    void exeTurn();
    void exeFind();
    void exeChase();
    void exeFall();
    void exeLand();
    void exeAttack();
    void exeCapHit();
    void exeBlowDown();

private:
    ExternalForceKeeper* mForceKeeper = nullptr;
    EnemyStateWander* mStateWander = nullptr;
    sead::Vector3f mCapPos = sead::Vector3f::zero;
    s32 mCapHitCooldown = 0;
    s32 mAirTime = 0;
    sead::Vector3f mFuturePos = sead::Vector3f::zero;
    sead::Vector3f mGroundNormal = sead::Vector3f::zero;
    s32 mWanderCooldown = 0;
};
