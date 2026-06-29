#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

class BossKnuckleThrowIce : public al::LiveActor {
public:
    BossKnuckleThrowIce(const char* name);
    void init(const al::ActorInitInfo& initInfo) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    void appear() override;
    void appearOnHand(const al::LiveActor* hand);
    void doBreak();
    void doThrow(const sead::Vector3f& velocity);
    bool isInAir() const;
    bool isBreak() const;
    void exeAppear();
    void exeWait();
    void exeFly();
    void exeBlowDown();
    void exeBreak();

private:
    const al::LiveActor* mHand = nullptr;
    sead::Vector3f mThrowVelocity = sead::Vector3f::zero;
};

static_assert(sizeof(BossKnuckleThrowIce) == 0x120);
