#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}

class KoopaShell;

class KoopaStateAttackSpinShell : public al::ActorStateBase {
public:
    KoopaStateAttackSpinShell(al::LiveActor* actor, KoopaShell* shell);
    void appear() override;
    void startOnGround();
    void startRecover();
    void exeAttackShellStart();
    void exeAttackShell();
    void exeAttackShellEnd();

private:
    KoopaShell* mShell = nullptr;
    bool mIsRecover = false;
};

static_assert(sizeof(KoopaStateAttackSpinShell) == 0x30);
