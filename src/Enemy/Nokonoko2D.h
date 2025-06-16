#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

namespace al {
class CollisionPartsFilterBase;
}

class Nokonoko2D : public al::LiveActor, public IUseDimension {
public:
    Nokonoko2D(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    void reset();
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void startMove();
    void control() override;

    void exeWalk();
    void exeKouraMove();
    void exeKouraMoveNoTouch();
    void exeDamage();
    void exeDamagePoison();
    void exeKouraStop();
    void exeKouraEnd();
    void exeHideWait();
    void exeAppearWait();

    ActorDimensionKeeper* getActorDimensionKeeper() const override { return mDimensionKeeper; }

private:
    ActorDimensionKeeper* mDimensionKeeper = nullptr;
    al::CollisionPartsFilterBase* mCollisionPartsFilter;
    bool mIsGreen = false;
    f32 mVelocityFront = 0.0f;
    bool mIsVertFlipped = false;
    f32 mLocalZRotator = 0.0f;
    s32 mKouraMoveOffscreenTimer = 0;
    sead::Vector3f mInitTrans = sead::Vector3f::zero;
    sead::Vector3f mInitFront = sead::Vector3f::zero;
    al::AreaObjGroup* mResetArea = nullptr;
    bool mIsResetForce = false;
    s32 mClippingTime = 300;
    bool mIsHeavyGravity = false;
    bool mIsTriggerTrampled = false;

    inline void setVertFlip();
    inline void clearVertFlip();
    inline bool isInClippingFrustum() const;
};

static_assert(sizeof(Nokonoko2D) == 0x168);
