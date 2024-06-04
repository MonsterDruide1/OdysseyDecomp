#include "Library/Movement/EnemyStateBlowDownParam.h"

namespace al {
EnemyStateBlowDownParam::EnemyStateBlowDownParam() = default;

EnemyStateBlowDownParam::EnemyStateBlowDownParam(const char* actionName)
    : mActionName(actionName) {}

EnemyStateBlowDownParam::EnemyStateBlowDownParam(const char* actionName, f32 gravityStrength,
                                                 f32 velocityStrength, f32 velocityMultiplier,
                                                 f32 velocityScale, s32 blowDownLength,
                                                 bool faceAwayFromActor)
    : mActionName(actionName), mGravityStrength(gravityStrength),
      mVelocityStrength(velocityStrength), mVelocityMultiplier(velocityMultiplier),
      mVelocityScale(velocityScale), mBlowDownLength(blowDownLength),
      mFaceAwayFromActor(faceAwayFromActor) {}

}  // namespace al
