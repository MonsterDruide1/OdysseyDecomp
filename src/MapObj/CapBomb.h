#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class LayoutActor;
}

class CapBomb : public al::LiveActor {
public:
    CapBomb(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;

    void exeWait();
    void exeExplosion();

private:
    al::LayoutActor* mTestLayout = nullptr;
    al::LayoutActor* mTestLayoutParts = nullptr;
};
