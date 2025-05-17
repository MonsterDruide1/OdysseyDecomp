#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class ParabolicPath;
}

class PackunPoisonBall : public al::LiveActor {
public:
    PackunPoisonBall(al::LiveActor* parent, bool isBig);

    void init(const al::ActorInitInfo& info) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void appear() override;

    void killBySwitch();
    void setParam(const sead::Vector3f& trans, const sead::Quatf& quat, bool isHack, f32 shootDist,
                  f32 shootHorizontalSpeed, f32 shootHeight);

    void exeMove();
    void exeFall();
    void exePaint();

private:
    al::LiveActor* mParent;
    bool mIsBig;
    al::ParabolicPath* mParabolicPath;
    s32 mParabolicPathTime = 0;
    sead::Vector3f mTrans = sead::Vector3f::zero;
    bool mIsHack = false;
    f32 mPaintAngle = 0.0f;
    f32 mShootDist = 200.0f;
    f32 mShootHorizontalSpeed = 8.0f;
    f32 mShootHeight = 50.0f;
    sead::Matrix34f mEffectCollidedWallMtx = sead::Matrix34f::ident;
};

static_assert(sizeof(PackunPoisonBall) == 0x178);
