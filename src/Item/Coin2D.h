#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

class Coin2D : public al::LiveActor, public IUseDimension {
public:
    Coin2D(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    void initAfterPlacement() override;
    void appear() override;
    void control() override;
    void endClipped() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    ActorDimensionKeeper* getActorDimensionKeeper() const override;

    void listenAppear();
    void appearCountUp();
    void exeAppear();
    void exeWait();
    void exeGot();
    void exeCountUp();
    void get();
    bool isGot() const;

private:
    char _110[0x18];
};
