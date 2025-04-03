#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class MtxConnector;
class PlacementId;
class SensorMsg;
}  // namespace al

class ActorDimensionKeeper;
class CoinCollectHintState;

class CoinCollect2D : public al::LiveActor, public IUseDimension {
public:
    CoinCollect2D(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    void initAfterPlacement() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void endClipped() override;

    ActorDimensionKeeper* getActorDimensionKeeper() const override;

    void appearHintEffect();
    void deleteHintEffect();
    void reappearHintEffect();
    bool isEnableHint() const;

    void exeWait();
    void exeWaitHint();
    void exeGot();

private:
    CoinCollectHintState* mHintState = nullptr;
    al::PlacementId* mPlacementId = nullptr;
    al::MtxConnector* mMtxConnector = nullptr;
    ActorDimensionKeeper* mDimensionKeeper = nullptr;
};
