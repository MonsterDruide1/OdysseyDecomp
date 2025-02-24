#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}

namespace alSensorFunction {

void updateHitSensorsAll(al::LiveActor*);
void clearHitSensors(al::LiveActor*);

u32 findSensorTypeByName(const char*);

}  // namespace alSensorFunction
