#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class HitSensor;
class SensorMsg;
}  // namespace al

class KillerStateHack : public al::ActorStateBase {
public:
    KillerStateHack(al::LiveActor*, bool, bool);

    void appear() override;
    void kill() override;

    void endHackExplode();
    bool receiveMsgInitCapTargetInfo(const al::SensorMsg* message);
    bool receiveMsgNpcScareByEnemy(const al::SensorMsg* message);
    bool receiveMsgHackStart(const al::SensorMsg* message, al::HitSensor* other,
                             al::HitSensor* self);
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);
    bool receiveMsgHackEndExplode(const al::SensorMsg* message, al::HitSensor* other,
                                  al::HitSensor* self);
    bool receiveMsgHackEnd(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);
    bool attackSensorCheckExplode(al::HitSensor* self, al::HitSensor* other);
    bool isStarting() const;
    bool isEnableExplode() const;
    bool isBoosting() const;
    bool isTurbo() const;
    void exeStart();
    void exeFly();

private:
    s8 padding[0x58 - sizeof(al::ActorStateBase)];
};

static_assert(sizeof(KillerStateHack) == 0x58);
