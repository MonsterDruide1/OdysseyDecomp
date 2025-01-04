#pragma once

#include <math/seadMatrix.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class ActorInitInfo;
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
    CoinStateCountUp* mCoinStateCountUp = nullptr;
    ExternalForceKeeper* mExternalForceKeeper = nullptr;
    CoinRotateCalculator* mCoinRotateCalculator = nullptr;
    al::MtxConnector* mMtxConnector = nullptr;
    const char* mArchiveName = nullptr;
    sead::Matrix34f mMatrix = sead::Matrix34f::ident;
    bool mIsNotInWater = false;
};
