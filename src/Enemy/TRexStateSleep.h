#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al

class TRexStateSleep : public al::ActorStateBase {
public:
    TRexStateSleep(const char* name, al::LiveActor* actor);

    void appear() override;

    bool receiveMsg(bool* outResult, const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self);

    void exeSleep();
    void exeSleepReactionHipDrop();
};
