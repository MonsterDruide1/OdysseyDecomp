#pragma once

#include "Library/Nerve/NerveStateBase.h"

class KillerStateHack : public al::ActorStateBase {
public:
    KillerStateHack(al::LiveActor*, bool, bool);

    bool attackSensorCheckExplode(al::HitSensor* self, al::HitSensor* other);
    bool isEnableExplode() const;
    void endHackExplode();
    bool receiveMsgHackEnd(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);
    bool receiveMsgHackEndExplode(const al::SensorMsg* message, al::HitSensor* other,
                                  al::HitSensor* self);
    bool receiveMsgHackStart(const al::SensorMsg* message, al::HitSensor* other,
                             al::HitSensor* self);
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);
    bool receiveMsgInitCapTargetInfo(const al::SensorMsg* message);
    bool receiveMsgNpcScareByEnemy(const al::SensorMsg* message);

private:
    s8 padding[0x58 - sizeof(al::ActorStateBase)];
};

static_assert(sizeof(KillerStateHack) == 0x58);
