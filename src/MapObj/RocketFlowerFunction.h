#pragma once

namespace al {
struct ActorInitInfo;
class HitSensor;
class LiveActor;
}  // namespace al

class RocketFlower;

namespace RocketFlowerFunction {

void createRocketFlowerEquipWatcherIfNotExist(const al::LiveActor* actor,
                                              const al::ActorInitInfo& info);
bool requestEquipRocketFlower(RocketFlower* flower, al::HitSensor* sensor);

}  // namespace RocketFlowerFunction
