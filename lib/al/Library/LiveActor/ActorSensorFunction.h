#pragma once

namespace al {
class HitSensorKeeper;
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al

namespace alActorSensorFunction {
al::HitSensorKeeper* getSensorKeeper(const al::LiveActor* actor);
// NOTE: The order of sensors here is the opposite of sendMsg functions
bool sendMsgSensorToSensor(const al::SensorMsg& message, al::HitSensor* sender,
                           al::HitSensor* receiver);
bool sendMsgToActorUnusedSensor(const al::SensorMsg& message, al::LiveActor* receiverActor);
}  // namespace alActorSensorFunction
