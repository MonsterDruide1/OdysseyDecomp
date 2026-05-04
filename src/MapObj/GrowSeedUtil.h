#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class HitSensor;
class LiveActor;
}  // namespace al

class GrowSeedStateWait;

namespace GrowSeedUtil {

const char* getGuideLabelName();
bool isInDeathArea(const al::LiveActor*);
void throwToDirection(al::LiveActor*, const sead::Vector3f&, f32);
void throwFromPlayer(al::LiveActor*, f32, f32);
f32 getPlantPartsAddRadius();
void rotateRandom(sead::Quatf*);
void rotateRandomY(sead::Quatf*);
void setVelocityForBlow(al::LiveActor*, al::HitSensor*, al::HitSensor*, f32, f32, f32);
void setVelocityForReflect(al::LiveActor*, f32, f32);
bool checkStrikeArrowBetweenPlayer(al::LiveActor*);
bool tryEmitEffectWaterSurface(bool, sead::Vector3f*, al::LiveActor*);
void prepareKillByShineGet(al::LiveActor*, const sead::Vector3f&);
bool isEnableSeedAttackSpeed(const al::LiveActor*, GrowSeedStateWait*, GrowSeedStateWait*, f32);

}  // namespace GrowSeedUtil
