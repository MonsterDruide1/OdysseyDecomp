#pragma once

#include "Library/LiveActor/LiveActor.h"

class EnemyStateDamageCap;
class KillerStateHack;

class Killer : public al::LiveActor {
public:
    Killer(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;

    bool isHack() const;

    void explode();
    void resetAliveCountAndAnim();
    void appearBy2D(const sead::Vector3f& position, const sead::Vector3f& velocity,
                    const sead::Quatf& rotation, s32 aliveFrame);
    void appearInit();
    void standByAppear(const sead::Vector3f& position, const sead::Quatf& rotation);
    void launch(s32 aliveFrame);
    void forceExplode();
    void applyVelocityDamp();

    void exeAppear();
    void exeStandBy();
    void exeLaunch();
    void exeFly();
    void exeExplode();
    void exeDamageCap();
    void exeHack();
    void exeAfterHack();
    void exeFallDown();

private:
    EnemyStateDamageCap* mEnemyStateDamageCap = nullptr;
    KillerStateHack* mStateHack = nullptr;
    s32 mAliveFrame = 1200;
    f32 mExplosionTime = 1200.0f;
    s32 mHackEndImmuneTime = 0;
    sead::Vector3f mLastFrontDir = sead::Vector3f::zero;
    f32 mRotationAngle = 0.0f;
    bool mCanExplode = true;
    bool mIsFirstCapture = true;
    bool mIsMagnum = false;
    bool mIsNoCap = false;
    bool mIsUseCaptureLight = false;
    bool mIsCapKoopa = false;
    s32 mAttackCooldown = 0;
};

static_assert(sizeof(Killer) == 0x140);
