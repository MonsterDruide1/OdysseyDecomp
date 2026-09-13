#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class HitSensor;
class LiveActor;
}  // namespace al

class HackerJudgeNormalFall;
class HackerStateWingFly;
class IUsePlayerHack;

class KuriboWingHackState : public al::ActorStateBase {
public:
    KuriboWingHackState(al::LiveActor* actor, IUsePlayerHack** playerHack);

    void appear() override;
    void control() override;
    void kill() override;

    void exeWait();
    void exeWalk();
    void exeLand();
    void exeFly();
    void exeFall();
    void exeAttackTrample();
    void attackSensor(al::HitSensor* self, al::HitSensor* other);
    void resetFlyLimit(const sead::Vector3f& flyLimit);

private:
    IUsePlayerHack** mPlayerHack = nullptr;
    HackerStateWingFly* mStateWingFly = nullptr;
    HackerJudgeNormalFall* mJudgeNormalFall = nullptr;
    f32 mTurnSpeed = 0.0f;
};

static_assert(sizeof(KuriboWingHackState) == 0x40);
