#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

class Togezo2D : public al::LiveActor, public IUseDimension {
public:
    Togezo2D(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    void control() override;
    void startMove();

    void exeWalk();
    void exeDamage();
    void exeHideWait();

    ActorDimensionKeeper* getActorDimensionKeeper() const override;

    inline void checkFacingTarget(al::HitSensor* target);

private:
    ActorDimensionKeeper* mDimensionKeeper = nullptr;
    f32 mVelocityFront = 0.0f;
    bool mIsAlwaysFalse = false;
    f32 mLocalZRotator = 0.0f;
    sead::Vector3f mInitTrans = sead::Vector3f::zero;
    sead::Vector3f mInitFront = sead::Vector3f::zero;
};

static_assert(sizeof(Togezo2D) == 0x140);
