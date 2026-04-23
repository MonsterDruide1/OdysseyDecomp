#pragma once

#include <math/seadMatrix.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class CameraTicket;
}  // namespace al

class Shine;

class SkyWorldKoopaFire : public al::LiveActor {
public:
    SkyWorldKoopaFire(const char* actorName);

    void init(const al::ActorInitInfo& info) override;
    void listenAppear();
    void listenKill();
    void exeWait();
    void exeHighTension();
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

private:
    sead::Matrix34f mEffectEmitPos = sead::Matrix34f::ident;
    s32 mCapAttackCooldown = 0;
};

static_assert(sizeof(SkyWorldKoopaFire) == 0x140);

class SkyWorldKoopaFrame : public al::LiveActor {
public:
    SkyWorldKoopaFrame(const char* actorName);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void exeWait();
    void exeReaction();
    void exeFall();
    void exeFallNoCollider();
    void exeFallEndWait();

private:
    al::CameraTicket* mDemoCamera = nullptr;
    Shine* mShine = nullptr;
};

static_assert(sizeof(SkyWorldKoopaFrame) == 0x118);
