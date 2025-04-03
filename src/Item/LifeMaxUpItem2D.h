#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
}  // namespace al

class ActorDimensionKeeper;
class FlashingCtrl;
class LifeUpItemStateAutoGet;

class LifeMaxUpItem2D : public al::LiveActor, public IUseDimension {
public:
    LifeMaxUpItem2D(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    ActorDimensionKeeper* getActorDimensionKeeper() const override;
    void appear() override;

    void appearPopUp();
    void appearByAmiibo(const sead::Vector3f&, const sead::Vector3f&);

    void exeAppeared();
    void exeSink();
    void exeGotWaitLifeUpDemo();
    void exeGotAppearCoin();
    void exeVanishing();
    void exeDeadWait();
    void exeAutoGetDemo();

private:
    LifeUpItemStateAutoGet* mStateAutoGet = nullptr;
    ActorDimensionKeeper* mDimensionKeeper = nullptr;
    FlashingCtrl* mFlashingCtrl = nullptr;
    s32 _128 = 0;
};
