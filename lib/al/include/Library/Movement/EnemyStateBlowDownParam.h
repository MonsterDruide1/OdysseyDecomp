#pragma once

#include <basis/seadTypes.h>

namespace al {
struct EnemyStateBlowDownParam {
    EnemyStateBlowDownParam();
    EnemyStateBlowDownParam(const char* actionName);
    EnemyStateBlowDownParam(const char* actionName, f32 velocityStrength, f32 gravityStrength,
                            f32 velocityMultiplier, f32 velocityScale, s32 blowDownLength,
                            bool faceAwayFromActor);

    const char* mActionName = "BlowDown";
    f32 mVelocityStrength = 10.3f;
    f32 mGravityStrength = 28.2f;
    f32 mVelocityMultiplier = 1.1f;
    f32 mVelocityScale = 0.995f;
    s32 mBlowDownLength = 120;
    bool mFaceAwayFromActor = true;
};
}  // namespace al
