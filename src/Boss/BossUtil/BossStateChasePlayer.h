#pragma once

#include "Library/Nerve/NerveStateBase.h"

struct BossStateChasePlayerParam {
    f32 acceleration;
    f32 deceleration;
    f32 scaleVelocity;
    f32 proximityStopDist;
    f32 turnSpeedSlow;
    f32 turnSpeedFast;
    f32 turnConvergeSpeed;
    f32 frontAngleThreshold;
    f32 chaseAngleMax;
};

class BossStateChasePlayer : public al::ActorStateBase {
public:
    BossStateChasePlayer(const char* name, al::LiveActor* actor,
                         const BossStateChasePlayerParam* param);

    void appear() override;

    void startStop();

    void exeChase();
    void exeLost();
    void exeStop();

private:
    const BossStateChasePlayerParam* mParam = nullptr;
    const al::LiveActor* mTargetPlayer = nullptr;
    f32 mRotationAngle = 0.0f;
};
