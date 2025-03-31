#pragma once

#include <math/seadMatrix.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class MtxConnector;
class SensorMsg;
}  // namespace al

class ActorDimensionKeeper;
class CoinStateCountUp;
class ExternalForceKeeper;
class CoinRotateCalculator;

class CoinCollectEmpty : public al::LiveActor {
public:
    CoinCollectEmpty(const char* name, const char* archiveName);

    void init(const al::ActorInitInfo& initInfo) override;
    void initAfterPlacement() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void endClipped() override;
    void appear() override;
    void control() override;

    void rotate();

    void exeWait();
    void exeGot();
    void exeCountUp();

private:
    CoinStateCountUp* mStateCountUp = nullptr;
    ExternalForceKeeper* mExternalForceKeeper = nullptr;
    CoinRotateCalculator* mRotateCalculator = nullptr;
    al::MtxConnector* mMtxConnector = nullptr;
    const char* mArchiveName = nullptr;
    sead::Matrix34f mWaterSurfaceMatrix = sead::Matrix34f::ident;
    bool mIsNotInWater = false;
};
