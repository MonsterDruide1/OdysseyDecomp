#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class HitSensor;
class LiveActor;

struct EnemyStateBlowDownParam {
public:
    EnemyStateBlowDownParam();
    EnemyStateBlowDownParam(const char* actionName);
    EnemyStateBlowDownParam(const char* actionName, f32 velocityStrength, f32 gravityStrength,
                            f32 velocityMultiplier, f32 velocityScale, s32 blowDownLength,
                            bool isFaceAwayFromActor);

    const char* actionName = "BlowDown";
    f32 velocityStrength = 10.3f;
    f32 gravityStrength = 28.2f;
    f32 velocityMultiplier = 1.1f;
    f32 velocityScale = 0.995f;
    s32 blowDownLength = 120;
    bool isFaceAwayFromActor = true;
};

class EnemyStateBlowDown : public ActorStateBase {
public:
    EnemyStateBlowDown(LiveActor* actor, const EnemyStateBlowDownParam* param, const char* name);
    void start(const HitSensor* other);
    void start(const sead::Vector3f& dir);
    void start(const HitSensor* other, const HitSensor* self);
    void start(const LiveActor* attacker);

    void appear() override;
    void kill() override;
    void control() override;

    void setParam(const EnemyStateBlowDownParam* param) { mParam = param; };

private:
    const EnemyStateBlowDownParam* mParam = nullptr;
    s32 mBlowDownTimer = 0;
    bool mIsInvalidClipping = false;
};
}  // namespace al
