#pragma once

#include <container/seadPtrArray.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class HosuiWaterBall : public al::LiveActor {
public:
    HosuiWaterBall();

    void init(const al::ActorInitInfo& initInfo) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void hitKill(al::HitSensor* receiver, al::HitSensor* sender);
    void clearIgnoreActors();
    void addIgnoreActor(const al::LiveActor* actor);
    void shoot(const sead::Vector3f& pos, const sead::Vector3f& velocity, u32 life,
               al::HitSensor* sender, bool isStrong);
    void exeShoot();
    void calcEffectMtx(const sead::Vector3f& pos, const sead::Vector3f& dir);

private:
    sead::FixedPtrArray<const al::LiveActor, 2> mIgnoreActors;
    s32 mLife = 0;
    al::HitSensor* mSender = nullptr;
    sead::Matrix34f mEffectMtx = sead::Matrix34f::ident;
    bool mIsStrong = false;
    u32 mShootStep = 0;
    al::HitSensor* mAttackSensor = nullptr;
};

static_assert(sizeof(HosuiWaterBall) == 0x178);
