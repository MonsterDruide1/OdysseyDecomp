#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class Car : public al::LiveActor {
public:
    Car(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void kill() override;
    void calcAnim() override;
    void movement() override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void exeWait();
    void exeRun();
    void tryBrake();
    void exeBrake();
    void exeStop();

    void setWatcherArray(sead::PtrArray<Car>* watcherArray) { mWatcherArray = watcherArray; }

private:
    u8 _108[0x68]{};
    sead::PtrArray<Car>* mWatcherArray = nullptr;
};

static_assert(sizeof(Car) == 0x178);
