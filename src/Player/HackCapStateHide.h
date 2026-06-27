#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}

class HackCap;

class HackCapStateHide : public al::ActorStateBase {
public:
    HackCapStateHide(HackCap* hackCap);

    void appear() override;

    void exeHide();

private:
    HackCap* mHackCap;
};
