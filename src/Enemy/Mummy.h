#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class JointSpringControllerHolder;
}
class Shine;

class Mummy : public al::LiveActor {
public:
    Mummy(const char* name);

    void init(const al::ActorInitInfo& info) override;

    inline void setupEffectMatrix();
    inline void checkEffects();
    inline void adjustVelocity();
    inline bool isAsleep() const;

    void startSleep();
    void attackSensor(al::HitSensor* other, al::HitSensor* self) override;
    bool isHide();
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* self,
                    al::HitSensor* other) override;
    void control() override;

    void setKnuckleMode();
    void appearWithTrans(const sead::Vector3f& trans);
    void setupEffect();
    void appearByTreasureBox(const sead::Vector3f& trans);
    void startWalkByTreasureBox();

    void sleep();
    void exeSleepShineStop();
    void exeSleepShine();
    void exeSleep();
    void exeWait();
    void exeAppear();
    void exeAppearAir();
    void exeWalkStart();
    void walk();
    void exeWalk();
    inline bool isOnSpecialGround() const;
    bool isHideByTimeLimit() const;
    void exeHeadLost();
    void exeHalfReaction();
    void exeHideStart();
    void exeHide();
    void exeBlowDown();

private:
    al::JointSpringControllerHolder* mJointSpringControllerHolder = nullptr;
    Shine* mShineActor = nullptr;
    f32 mWalkDirectionOffset = 0.0f;
    s32 mWalkDirectionChangeTimer = 60;
    s32 mHeadRegenTimer = 0;
    s32 mEffectTimer = 0;
    s32 mTimeLimit = 0;
    s32 mInvulnerableTimer = 0;
    sead::Matrix34f mEffectMatrix = sead::Matrix34f::ident;
    bool mIsHeadLost = false;
    bool mHasCoin = false;
    bool mIsTreasureBoxSpawned = false;
    bool mIsNoHitStop = false;
};
