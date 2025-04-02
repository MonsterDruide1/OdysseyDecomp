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

class CoinCollectEmpty2D : public al::LiveActor, public IUseDimension {
public:
    CoinCollectEmpty2D(const char* name, const char* archiveName);

    void init(const al::ActorInitInfo& initInfo) override;
    void initAfterPlacement() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void endClipped() override;

    ActorDimensionKeeper* getActorDimensionKeeper() const override;

    void exeWait();
    void exeGot();

private:
    al::MtxConnector* mMtxConnector = nullptr;
    ActorDimensionKeeper* mDimensionKeeper = nullptr;
    const char* mArchiveName = nullptr;
};
