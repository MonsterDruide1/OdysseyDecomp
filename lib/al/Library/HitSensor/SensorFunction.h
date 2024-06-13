#pragma once

namespace al {
class LiveActor;
}

namespace alSensorFunction {

void updateHitSensorsAll(al::LiveActor*);
void clearHitSensors(al::LiveActor*);

// TODO: find this type
void findSensorTypeByName(const char*);

}  // namespace alSensorFunction
