#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class ParabolicPath;
}

class BossRaidWheel : public al::LiveActor {
public:
    BossRaidWheel(const char* name);
    void init(const al::ActorInitInfo& info) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self) override;
    void control() override;

    void exeLand();
    void exeRun();

    void shotGround(const sead::Vector3f& pos, const sead::Vector3f& dir, f32 earlyTurn);

private:
    al::ParabolicPath* mPath = nullptr;
    sead::Vector3f mFallSignPos = sead::Vector3f::zero;
    s32 mRunTurnOffset = 0;
    f32 mRotateSpeed = 5.0f;
    f32 mRotate = 0.0f;
    f32 mTilt = 0.0f;
};

static_assert(sizeof(BossRaidWheel) == 0x130);
