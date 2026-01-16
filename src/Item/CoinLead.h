#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
}  // namespace al

class Coin;

class CoinLead : public al::LiveActor {
public:
    CoinLead(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void place(const sead::Vector3f& pos, s32 index, f32 coord);
    void exeWait();
    void exeFirstCoinGotDelay();
    void exeMove();
    void exeWaitAllGet();

private:
    Coin* mCoinLead = nullptr;
    Coin** mCoinArray = nullptr;
    f32* mCoinCoords = nullptr;
    s32 mCoinCount = 0;
    f32 mRailTotalLength = 0;
    f32 mSpeed = 0.0f;
    f32 mDistance = 0.0f;
    s32 mCoinsDisplayed = 1;
    f32 mPlaceInterval = 0.0f;
    sead::Vector3f mClippingInfo = sead::Vector3f::zero;
    sead::Vector3f mDisplayOffset = sead::Vector3f::zero;
    bool mIsPlaySuccessSe = true;
};

static_assert(sizeof(CoinLead) == 0x158);
