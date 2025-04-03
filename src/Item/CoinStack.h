#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class CoinStackGroup;

class CoinStack : public al::LiveActor {
public:
    CoinStack(const char* name);
    virtual ~CoinStack();

    void init(const al::ActorInitInfo& initInfo) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    CoinStack* getBelow();
    CoinStack* getAbove();
    void makeStackAppear();
    void makeStackDisappear();
    void changeAlpha(f32 alphaMask);
    f32 getFallSpeed();
    void setAbove(CoinStack* stack);
    void setBelow(CoinStack* stack);
    void signalFall(u32 delay, f32 speed);
    void postInit(CoinStackGroup* coinStackGroup, const sead::Vector3f& transY, CoinStack* below,
                  const sead::Vector3f& clippingPos, f32 clippingRadius, const f32* fallDistance);

    void exeWait();
    void exeFloat();
    void exeFall();
    void exeLand();
    void exeCollected();

    f32 getTransY() const { return mTransY; }

private:
    const f32* mExternalFallDistance = nullptr;
    f32 mTransY = 0.0f;
    f32 mLandHeight = 0.0f;
    s32 mFloatDuration = 0;
    f32 mFallSpeed = 0.0f;
    CoinStackGroup* mCoinStackGroup = nullptr;
    sead::Vector3f mClippingPos;
    f32 mClippingRadius = 0.0f;
    CoinStack* mStackBelow = nullptr;
    CoinStack* mStackAbove = nullptr;
};
