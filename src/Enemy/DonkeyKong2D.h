#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

namespace al {
struct ActorInitInfo;
template <typename T>
class DeriveActorGroup;
class HitSensor;
class SensorMsg;
}  // namespace al
class ActorDimensionKeeper;
class Barrel2D;
class BarrelStack2D;

class DonkeyKong2D : public al::LiveActor, public IUseDimension {
public:
    DonkeyKong2D(const char* name);
    void init(const al::ActorInitInfo& info) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void exeWait();
    void exeThrow();
    void exeDamage();
    void exeDown();

    ActorDimensionKeeper* getActorDimensionKeeper() const override { return mDimensionKeeper; }

private:
    ActorDimensionKeeper* mDimensionKeeper = nullptr;
    al::DeriveActorGroup<Barrel2D>* mBarrelGroup = nullptr;
    Barrel2D* mHeldBarrel = nullptr;
    BarrelStack2D* mBarrelStack = nullptr;
    s32 mWaitStep = 180;
    f32 mMoveSpeed = 10.0f;
    s32 mHitsTaken = 0;
};

static_assert(sizeof(DonkeyKong2D) == 0x140);
