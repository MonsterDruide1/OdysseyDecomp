#pragma once

#include <basis/seadTypes.h>
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
                            bool faceAwayFromActor);

    const char* actionName = "BlowDown";
    f32 velocityStrength = 10.3f;
    f32 gravityStrength = 28.2f;
    f32 velocityMultiplier = 1.1f;
    f32 velocityScale = 0.995f;
    s32 blowDownLength = 120;
    bool faceAwayFromActor = true;
};

class EnemyStateBlowDown : public ActorStateBase {
public:
    EnemyStateBlowDown(LiveActor*, const EnemyStateBlowDownParam*, const char*);
    void start(const HitSensor*);
    void start(const sead::Vector3f&);
    void start(const HitSensor*, const HitSensor*);
    void start(const LiveActor*);

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
