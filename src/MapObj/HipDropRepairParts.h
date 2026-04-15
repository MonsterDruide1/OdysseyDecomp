#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class ItemGenerator;

class HipDropRepairParts : public al::LiveActor {
public:
    HipDropRepairParts(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void startHide();
    void startShow();
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void exeWait();
    void exeOn();
    void exeOnWait();

private:
    al::LiveActor* mSubActor = nullptr;
    ItemGenerator* mItemGenerator = nullptr;
    sead::Vector3f mInitTrans = sead::Vector3f::zero;
    sead::Vector3f mItemGeneratePos = sead::Vector3f::zero;
};

static_assert(sizeof(HipDropRepairParts) == 0x130);
