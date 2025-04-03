#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
}  // namespace al

class LifeMaxUpItem : public al::LiveActor {
public:
    LifeMaxUpItem(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    void initAfterPlacement() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void appear() override;
    void control() override;

    void appearAmiiboTouch(const sead::Vector3f&);
    void appearPopUp();
    void appearPopUpAbove();
    void appearSlot();
    void appearPopUpDir(const sead::Quatf&);

    void exeAppeared();
    void exeStayPlacedPos();
    void exeWaterFallWorld();
    void exeAutoGetDemo();
    void exeGotWaitLifeUpDemo();
    void exeGotAppearCoin();
    void exeGotDeadWait();

private:
    char filler[0x48];
};
