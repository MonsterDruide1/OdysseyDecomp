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

    void registerCoin(Coin2DCity*);
    void getCoin();
    bool isTriggerBeat() const;

    void exeWait();

    s32 getLightTime() const { return mLightTime; }

private:
    sead::PtrArray<Coin2DCity> mCoinHolder;
    sead::Vector3f _118;
    s32 mNextCoinLightTime;
    s32 mLightTime;
    s32 mLightInterval;
    s32 _130;
    s32 mDelayTime;
    al::BgmBeatCounter* mBgmBeatCounter;
};
