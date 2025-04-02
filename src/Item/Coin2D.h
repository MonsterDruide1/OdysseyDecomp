#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class MtxConnector;
class SensorMsg;
}  // namespace al

class ActorDimensionKeeper;

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
    void get();
    bool isGot() const;

    void exeAppear();
    void exeWait();
    void exeGot();
    void exeCountUp();

private:
    al::MtxConnector* mMtxConnector = nullptr;
    ActorDimensionKeeper* mDimensionKeeper = nullptr;
    bool mIsConnectToCollisionBack = false;
    bool mIsPlaced = false;
};
