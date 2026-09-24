#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>

namespace al {
struct ActorInitInfo;
class LiveActor;
}  // namespace al

class BossForest;

namespace BossForestLocalFunction {
void createCollisionParts(BossForest* bossForest, const al::ActorInitInfo& initInfo,
                          sead::Matrix34f* collisionMtx);
void setVisAnimDomeDamage(al::LiveActor* actor, f32 domeDamage);
void appearLifeParts(al::LiveActor* actor);
void setupLifePartsEffectMtx(BossForest* bossForest, const sead::Matrix34f* jointMtx, s32 index);
void offEyesJointControl(BossForest* bossForest);
void onEyesJointControl(BossForest* bossForest);
s32 countActiveSatellitesNum(const BossForest* bossForest);
}  // namespace BossForestLocalFunction
