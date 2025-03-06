#pragma once

#include <container/seadPtrArray.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class ActorInitInfo;
class BgmBeatCounter;
class HitSensor;
class SensorMsg;
}  // namespace al

class Coin2DCity;

class Coin2DCityDirector : public al::LiveActor {
public:
    Coin2DCityDirector(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    void initAfterPlacement() override;
    void control() override;

    void registerCoin(Coin2DCity* coin);
    void getCoin();
    bool isTriggerBeat() const;

    void exeWait();

    s32 getLightTime() const { return mLightTime; }

private:
    sead::PtrArray<Coin2DCity> mCoinHolder;
    sead::Vector3f mClippingPos = sead::Vector3f::zero;
    s32 mNextCoinLightTime = 0;
    s32 mLightTime = 0;
    s32 mLightInterval = 0;
    s32 mCoinsCollected = 0;
    s32 mDelayTime = 0;
    al::BgmBeatCounter* mBgmBeatCounter = nullptr;
};
