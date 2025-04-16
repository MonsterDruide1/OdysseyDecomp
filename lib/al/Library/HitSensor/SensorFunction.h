#pragma once

namespace al {
class LiveActor;
enum class HitSensorType : u32;
}  // namespace al

namespace alSensorFunction {

void updateHitSensorsAll(al::LiveActor*);
void clearHitSensors(al::LiveActor*);

al::HitSensorType findSensorTypeByName(const char*);

}  // namespace alSensorFunction
