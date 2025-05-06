#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class RiseMapParts : public al::LiveActor {
public:
    RiseMapParts(const char*);
    void init(const al::ActorInitInfo&) override;
    void initAfterPlacement() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    bool isStop() const;
    void control() override;
    void setStop();
    void setStopPose();

    void exeWait();
    void exeMove();
    void start();
    void exePause();
    void exeStopBefore();
    void exeStop();
    void startRise();

private:
    void* filler[6];
};

static_assert(sizeof(RiseMapParts) == 0x138);
