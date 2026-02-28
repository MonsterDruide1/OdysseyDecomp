#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class HitInfo;
class ArrowHitInfo;
}  // namespace al

class TsukkunUtil {
public:
    bool isStickReflectCode(const al::HitInfo*);
};

class Tsukkun : public al::LiveActor {
public:
    Tsukkun(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void validateSensors();
    void setAnimHackOff();
    void start();
    void setAnimHackOn();
    void setBend(const sead::Vector3f&, const sead::Vector3f&, f32);
    void clearBend();
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool isEnableAttack() const;
    bool isEnablePush() const;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    bool isEnableDamage() const;
    bool isStateEnemy() const;
    void deathAndReset();
    bool isEnableCapLockOn() const;
    void startHack(al::HitSensor*, al::HitSensor*);
    bool isEnableCap() const;
    bool isEnableCapAttack() const;
    void blowDownCap(al::HitSensor*);
    void control() override;
    void calcCenterPos(sead::Vector3f*) const;
    void calcBeakPos(sead::Vector3f*) const;
    void calcStickPos(sead::Vector3f*, sead::Vector3f*) const;
    void appearBall(const sead::Vector3f&, const sead::Vector3f&);
    void appearCap();
    void exeStandBy();
    void exeRevive();
    void exeAppear();
    void exeAppearBall();
    void updateVelocity(f32, f32);
    void exeAppearBallEnd();
    void updateGround();
    void exeWait();
    void exeFind();
    void exeMove();
    void addMoveAccel(const sead::Vector3f&);
    void setMoveActionRate(f32);
    void exeThrustSign();
    void exeThrust();
    bool checkHitBeak(const al::ArrowHitInfo**, sead::Vector3f*) const;
    void exeThrustLoop();
    void exeThrustEnd();
    void exeThrustCancel();
    void exeDamageCap();
    void exeSwoon();
    void exeHack();
    void exeTrampleDown();
    void exeBlowDown();
    void exeReset();
    void updateVelocity();

private:
    char _108[0x180 - sizeof(al::LiveActor)];
};
