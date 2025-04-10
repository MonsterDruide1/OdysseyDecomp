#pragma once

#include <basis/seadTypes.h>

#include "Project/HitSensor/HitSensor.h"

namespace al {
class LiveActor;
}

namespace alSensorFunction {

void updateHitSensorsAll(al::LiveActor*);
void clearHitSensors(al::LiveActor*);

al::HitSensorType findSensorTypeByName(const char*);

}  // namespace alSensorFunction
