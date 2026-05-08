#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class DisregardReceiver;
class Shine;

class StatueSnapMark : public al::LiveActor {
public:
    StatueSnapMark(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeWait();
    void exeAppearShine();
    void exeDone();

private:
    Shine* mShineActor = nullptr;
    al::LiveActor* mSnapTarget = nullptr;
    DisregardReceiver* mDisregardReceiver = nullptr;
};

static_assert(sizeof(StatueSnapMark) == 0x120);
