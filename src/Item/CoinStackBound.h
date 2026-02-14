#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class FlashingTimer;
class HitSensor;
class SensorMsg;
class WaterSurfaceFinder;
}  // namespace al

class CoinStackBound : public al::LiveActor {
public:
    CoinStackBound(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void appear() override;
    void processTimer();

    void exeAlive();
    void exeAppear();
    void exeFall();
    void exeBounce();
    void exeVanish();
    void exeCollected();

private:
    char filler[0xc];
    sead::Vector3f mMoveDirH;
    sead::Vector3f mRotationAxis;
    sead::Vector3f mBounceNormal;
    sead::Quatf mQuat;
    s32 mBounceCount = 0;
    f32 mMaxVelocity = 17.5f;
    bool mHasReachedMaxVelocity = false;
    f32 mRotationSpeed = 30.0f;
    al::FlashingTimer* mFlashingTimer = nullptr;
    bool mIsTimerRush = false;
    sead::Matrix34f mBounceMtxEffect;
    bool mHasWaterCollision = false;
    al::WaterSurfaceFinder* mWaterSurfaceFinder = nullptr;
    bool mHasWallCollision = true;
};

static_assert(sizeof(CoinStackBound) == 0x1a8);
