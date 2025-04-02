#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class ActorDimensionKeeper;
class Coin2DCityDirector;

class Coin2DCity : public al::LiveActor, public IUseDimension {
public:
    Coin2DCity(const char* name, Coin2DCityDirector* director);

    void init(const al::ActorInitInfo& initInfo) override;
    void control() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    ActorDimensionKeeper* getActorDimensionKeeper() const override;

    void startLight();
    void endLight();
    void exeWait();
    void exeLight();
    void exeGot();
    void exeGotWait();

private:
    ActorDimensionKeeper* mDimensionKeeper = nullptr;
    Coin2DCity* mNextCoin = nullptr;
    Coin2DCityDirector* mCityDirector = nullptr;
    s32 mLightTime = -1;
    s32 mSyncCounter = 0;
};
