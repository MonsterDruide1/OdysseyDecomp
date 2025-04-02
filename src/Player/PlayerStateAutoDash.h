#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}

class PlayerStateAutoDash : public al::ActorStateBase {
public:
    PlayerStateAutoDash(al::LiveActor* parent);

    void appear();
    void exeDash();
    void exeRun();

private:
    s32 mRunTimer = 0;
};
