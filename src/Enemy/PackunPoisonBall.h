#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class ParabolicPath;
}

class PackunPoisonBall : public al::LiveActor {
public:
    PackunPoisonBall(al::LiveActor*, bool isBig);

    void init(const al::ActorInitInfo& info) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void appear() override;

    void killBySwitch();
    void setParam(const sead::Vector3f& trans, const sead::Quatf& quat, bool isHack, f32, f32, f32);

    void exeMove();
    void exeFall();
    void exePaint();

private:
    al::LiveActor* _108;
    bool mIsBig;
    al::ParabolicPath* _118;
    s32 _120 = 0;
    sead::Vector3f _124 = sead::Vector3f::zero;
    bool mIsHack = false;
    f32 _134 = 0.0f;
    f32 _138 = 200.0f;
    f32 _13c = 8.0f;
    f32 _140 = 50.0f;
    sead::Matrix34f mEffectCollidedWallMtx = sead::Matrix34f::ident;
};

static_assert(sizeof(PackunPoisonBall) == 0x178);
