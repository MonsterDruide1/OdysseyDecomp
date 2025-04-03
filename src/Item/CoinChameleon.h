#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class MtxConnector;
class SensorMsg;
}  // namespace al

class Coin;
class CoinRotateCalculator;
class CoinStateAppearRotate;

class CoinChameleon : public al::LiveActor {
public:
    CoinChameleon(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    void initAfterPlacement() override;
    void endClipped() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void rotate();

    void exeWait();
    void exeVisible();
    void exeAppear();

private:
    Coin* mCoin = nullptr;
    CoinStateAppearRotate* mStateAppearRotate = nullptr;
    sead::Quatf mQuat = sead::Quatf::unit;
    al::MtxConnector* mMtxConnector = nullptr;
    CoinRotateCalculator* mRotateCalculator = nullptr;
    sead::Vector3f mDisplayOffset = sead::Vector3f::zero;
    s32 mWaitTime = 0;
};
