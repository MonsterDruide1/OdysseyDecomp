#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class Souvenir : public al::LiveActor {
public:
    Souvenir(const char* name);
    void init(const al::ActorInitInfo& actorInitInfo) override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* msg, al::HitSensor* self, al::HitSensor* other) override;

    void exeWait();
    inline const char* getNextAction();
    void exeReactionCap();

private:
    bool mIsWait = false;
    bool mIsReactionPlayerUpperPunch = false;
    bool mIsThroughCapAttack = false;
    f32 mRotateYSpeed = 0.0f;
};

static_assert(sizeof(Souvenir) == 0x110);
