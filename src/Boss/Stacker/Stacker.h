#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "Library/Thread/FunctorV0M.h"

class MultiGateKeeperWatcher;

class Stacker : public al::LiveActor {
public:
    Stacker(const char*, MultiGateKeeperWatcher*);
    void init(const al::ActorInitInfo&) override;
    void startBattleStartDemo();
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* self, al::HitSensor* other,
                    al::HitSensor* hitSensor) override;
    bool blowCapOnHead(const sead::Vector3f&, const sead::Vector3f&);
    void control() override;
    void kill() override;
    void calcAnim() override;
    bool isAlreadyDead() const;
    void attackStartCap();
    void attackEndCap();
    void updateAnimFrameCapOnHead();
    void restStartCap();
    void restEndCap();
    void updateCapOnHead();
    void blowAllCapOnHead();
    void exeBeforeStartDemo();
    void exeDemoBattleStart();
    void resetCapOnHead();
    void exeNormalAttack();
    void exePanic();
    void exeDamage();
    void exeInvincibleAttack();
    void exeReset();
    void exeDemoBattleEnd();
};
