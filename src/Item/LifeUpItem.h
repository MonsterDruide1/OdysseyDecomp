#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class FireSurfaceFinder;
class HitSensor;
class MtxConnector;
class NerveStateBase;
class SensorMsg;
class WaterSurfaceFinder;
}  // namespace al

class LifeUpItemStateAutoGet;

class LifeUpItem : public al::LiveActor {
public:
    LifeUpItem(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;

    void appearPopUp();
    void appearPopUpBack();
    void appearPopUpAbove();
    void appearSlot();
    void appearInvalidClipping();
    void appearAmiiboTouch();
    void get();

    void exePopUp();
    void exeLand();
    void exeWait();
    void exeTakeWater();
    void exeLandWater();
    void exeFloatWater();
    void exeGet();
    void exeAutoGetDemo();

    void setBool145(bool value) { _145 = value; }

private:
    LifeUpItemStateAutoGet* mStateAutoGet = nullptr;
    al::HitSensor* mHitSensor = nullptr;
    al::MtxConnector* mMtxConnector = nullptr;
    const al::LiveActor* mConnectedHost = nullptr;
    bool mIsCreateMtxConnector = true;
    s32 mCoinAppearCount = 0;
    al::WaterSurfaceFinder* mWaterSurfaceFinder = nullptr;
    al::FireSurfaceFinder* mFireSurfaceFinder = nullptr;
    f32 mShadowMaskDropLength = 0.0f;
    bool mIsValidateClipping = true;
    bool _145 = false;
};

static_assert(sizeof(LifeUpItem) == 0x148);
