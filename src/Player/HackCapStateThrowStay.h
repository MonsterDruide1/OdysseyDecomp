#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}

class HackCap;

class HackCapStateThrowStay : public al::ActorStateBase {
public:
    HackCapStateThrowStay(HackCap* hackCap);

    void appear() override;

    void exeWait();
    void exeStay();
    void exeEnd();
    bool isHomingPlayerJump() const;

private:
    HackCap* mHackCap;
};
