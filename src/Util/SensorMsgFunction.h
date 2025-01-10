#pragma once

#include <math/seadVector.h>

namespace al {
class HitSensor;
class SensorMsg;
}  // namespace al

namespace rs {

bool isMsgAirExplosion(const al::SensorMsg* msg);
bool isMsgByugoBlow(const al::SensorMsg* msg);
bool isMsgCapAttack(const al::SensorMsg* msg);
bool isMsgFishingItemGet(const al::SensorMsg* msg);
bool isMsgFishingLineTouch(const al::SensorMsg* msg);
bool isMsgItemGet2D(const al::SensorMsg* msg);
bool isMsgItemGetAll(const al::SensorMsg* msg);
bool isMsgPlayerDisregardTargetMarker(const al::SensorMsg* msg);

bool tryGetAirExplosionForce(sead::Vector3f* force, const al::SensorMsg* msg);
bool tryGetByugoBlowForce(sead::Vector3f* force, const al::SensorMsg* msg);

void requestHitReactionToAttacker(const al::SensorMsg*, const al::HitSensor*,
                                  const sead::Vector3f&);

}  // namespace rs
