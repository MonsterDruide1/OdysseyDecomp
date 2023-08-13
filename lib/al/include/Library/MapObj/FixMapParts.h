#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class FixMapParts : public LiveActor {
public:
    FixMapParts(const char* name);
    void init(const ActorInitInfo& info) override;
    void appear() override;
    void movement() override;
    void calcAnim() override;
    bool receiveMsg(const SensorMsg* message, HitSensor* source, HitSensor* target) override;

private:
    bool mStatic = false;
};
}  // namespace al
