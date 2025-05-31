#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class Coin;

class CoinRail : public al::LiveActor {
public:
    CoinRail(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    void appear() override;
    void kill() override;
    void makeActorDead() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    bool isGot() const;

    void exeMove();
    void exeCloseMove();

private:
    Coin** mCoins = nullptr;
    f32* mRailPos = nullptr;
    s32 mCoinNum = 0;
    f32 mMoveVelocity = 0.0f;
    s32 mFirstCoinIndex = 0;
    s32 mLastCoinIndex = 0;
    sead::Vector3f mClippingInfo;
    sead::Vector3f mDisplayOffset = sead::Vector3f::zero;
};
