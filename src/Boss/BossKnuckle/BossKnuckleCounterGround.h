#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class BossKnuckleCounterGround : public al::LiveActor {
public:
    BossKnuckleCounterGround(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void doBreak();
    void doFall(const sead::Vector3f& trans);
    void doFallStartDemo();
    void setBeforeStart();
    void doWaitOnGround();
    bool isBreak() const;
    bool isBeforeStart() const;
    void killAll();
    void exeWaitOnGround();
    void exeFall();
    void exeSink();
    void exeBreak();
    void exeBeforeStart();

private:
    al::LiveActor* mBreakActor = nullptr;
    al::LiveActor* mUnusedActor = nullptr;
    al::LiveActor* mLifeUpItem = nullptr;
    sead::Vector3f mFallStartTrans = {0.0f, 0.0f, 0.0f};
    bool mIsBreakOnGround = false;
    bool mIsBreakByIceConflict = false;
};

static_assert(sizeof(BossKnuckleCounterGround) == 0x130);
