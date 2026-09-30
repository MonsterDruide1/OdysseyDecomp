#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}

class KoopaFlag;
class KoopaJumpMovement;
class KoopaLandPointHolder;
class KoopaRingBeamEmitter;

class KoopaStateHipDrop : public al::ActorStateBase {
public:
    KoopaStateHipDrop(al::LiveActor* actor, KoopaFlag* flag, KoopaRingBeamEmitter* ringBeamEmitter);

    void appear() override;
    void kill() override;

    void decideEitherFarSidePointAndStart(KoopaLandPointHolder* landPointHolder);
    bool isAfterHipDropLand() const;
    void exeJumpStart();
    void exeJump();
    void exeHipDropSign();
    void exeHipDropStart();
    void exeHipDrop();
    void exeHipDropLandStart();
    void exeHipDropLand();
    void exeHipDropLandEnd();

private:
    KoopaFlag* mFlag = nullptr;
    KoopaJumpMovement* mJumpMovement = nullptr;
    KoopaRingBeamEmitter* mRingBeamEmitter = nullptr;
};

static_assert(sizeof(KoopaStateHipDrop) == 0x38);
