#pragma once

#include "Library/Nerve/NerveStateBase.h"

class KillerStateHack : public al::ActorStateBase {
public:
    KillerStateHack(al::LiveActor*, bool, bool);

    bool attackSensorCheckExplode(al::HitSensor*, al::HitSensor*);
    bool isEnableExplode() const;
    void endHackExplode();

private:
    s8 padding[0x58 - sizeof(al::ActorStateBase)];
};

static_assert(sizeof(KillerStateHack) == 0x58);
