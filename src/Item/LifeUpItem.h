#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class MtxConnector;
class NerveStateBase;
class SensorMsg;
}  // namespace al

class WaterSurfaceFinder;
class FireSurfaceFinder;

class LifeUpItem : public al::LiveActor {
public:
    LifeUpItem(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
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
    al::NerveStateBase* mNerveStateBase = nullptr;
    al::HitSensor* mHitSensor = nullptr;
    al::MtxConnector* mMtxConnector = nullptr;
    al::LiveActor* _120 = nullptr;
    s32 _12c = 0;
    WaterSurfaceFinder* mWaterSurfaceFinder = nullptr;
    FireSurfaceFinder* mFireSurfaceFinder = nullptr;
    s32 mShadowMaskDropLength = 0;
    bool mIsValidateClipping = true;
    bool _145 = false;
};
