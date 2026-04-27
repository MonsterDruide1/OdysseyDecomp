#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class ActorDimensionKeeper;
class LifeUpItemStateAutoGet;

class LifeUpItem2D : public al::LiveActor, public IUseDimension {
public:
    LifeUpItem2D(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    ActorDimensionKeeper* getActorDimensionKeeper() const override;

    void appearPopUp();
    void appearAmiiboTouch();
    void get();

    void exePopUp();
    void exeWait();
    void exeGet();
    void exeAutoGetDemo();

    void setIsAmiiboTouch(bool value) { mIsAmiiboTouch = value; }

private:
    LifeUpItemStateAutoGet* mStateAutoGet = nullptr;
    al::HitSensor* mHitSensor = nullptr;
    s32 mCoinAppearCount = 0;
    ActorDimensionKeeper* mDimensionKeeper = nullptr;
    bool mIsAmiiboTouch = false;
};

static_assert(sizeof(LifeUpItem2D) == 0x138);
