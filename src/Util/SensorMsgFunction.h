#pragma once

#include <math/seadVector.h>

namespace al {
class HitSensor;
class SensorMsg;
}  // namespace al

namespace rs {

bool isMsgPlayerDisregardTargetMarker(const al::SensorMsg* msg);

void requestHitReactionToAttacker(const al::SensorMsg*, const al::HitSensor*,
                                  const sead::Vector3f&);

}  // namespace rs
