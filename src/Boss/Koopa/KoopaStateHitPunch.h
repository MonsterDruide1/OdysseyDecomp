#pragma once

#include "Library/Nerve/NerveStateBase.h"

#include "Boss/Koopa/KoopaFlag.h"
#include "Boss/Koopa/KoopaFunction.h"
#include "Boss/Koopa/KoopaNumberCounter.h"

namespace al {
class LiveActor;
}

class KoopaCap;
class KoopaFlag;
class KoopaNumberCounter;

class KoopaStateHitPunch : public al::ActorStateBase {
public:
    KoopaStateHitPunch(al::LiveActor* actor, KoopaCap* cap, KoopaFlag* flag,
                       KoopaNumberCounter* counter);
    void appear() override;
    void exeHitFirst();
    void exeHit();
    void exeHitEnd();

private:
    KoopaCap* mCap = nullptr;
    KoopaFlag* mFlag = nullptr;
    KoopaNumberCounter* mCounter = nullptr;
};
