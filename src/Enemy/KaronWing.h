#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

class CapTargetInfo;
class EnemyCap;
class EnemyStateSwoon;
class EnemyStateHackStart;
class FlyerStateWander;
class IUsePlayerHack;
class KaronWingStateHack;

class KaronWing : public al::LiveActor {
public:
    KaronWing(const char* name) : LiveActor(name) {}

    void init(const al::ActorInitInfo& info) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;
    void updateCollider() override;

    void exeAttack();
    void exeBreak();
    void exeChase();
    void exeDamageCap();
    void exeFind();
    void exeHack();
    void exeHackStart();
    void exeRevive();
    void exeReviveAppear();
    void exeSwoon();
    void exeTurn();
    void exeWait();
    void exeWander();

private:
    IUsePlayerHack* mPlayerHack = nullptr;
    CapTargetInfo* mCapTargetInfo = nullptr;
    KaronWingStateHack* mStateHack = nullptr;
    EnemyStateSwoon* mStateSwoon = nullptr;
    EnemyStateSwoon* mStateBreak = nullptr;
    EnemyStateHackStart* mStateHackStart = nullptr;
    FlyerStateWander* mStateWander = nullptr;
    EnemyCap* mEnemyCap = nullptr;
    f32 mDamageStartY = 0.0f;
    sead::Vector3f mSpawnTrans = sead::Vector3f::zero;
};
