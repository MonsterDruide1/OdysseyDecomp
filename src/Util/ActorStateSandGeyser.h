#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class SensorMsg;
class HitSensor;
}

class ActorStateSandGeyser : public al::ActorStateBase {
public:
    ActorStateSandGeyser(al::LiveActor*);

    void appear() override;
    void kill() override;

    void tryEndPadRumble();
    void exeSandGeyser();
    void tryStartPadRumble();
    bool receiveMsgSandGeyser(const al::SensorMsg*, al::HitSensor*);

private:
    // TODO
};
