#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

class DecalJointCtrl;

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class TimeBalloonCircle : public al::LiveActor {
public:
    TimeBalloonCircle();

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void setTrans(const sead::Vector3f& trans);
    void startStandby();
    void startPlay();
    void hideModel();

    void exeWait();
    void exeOut();
    void exeIn();

private:
    sead::Vector3f mTrans = sead::Vector3f::zero;
    DecalJointCtrl* mDecalJointCtrl = nullptr;
};

static_assert(sizeof(TimeBalloonCircle) == 0x120);
