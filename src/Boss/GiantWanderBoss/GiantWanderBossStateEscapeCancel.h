#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActorGroup.h"
#include "Library/Nerve/NerveStateBase.h"

class GiantWanderBoss;
class GiantWanderBossMine;

class GiantWanderBossStateEscapeCancel : public al::HostStateBase<GiantWanderBoss> {
public:
    GiantWanderBossStateEscapeCancel(GiantWanderBoss* boss,
                                     al::DeriveActorGroup<GiantWanderBossMine>* mineGroup);
    ~GiantWanderBossStateEscapeCancel() override;

    void appear() override;

    void exeAngry();
    void exeAppearMine();
    void exeLaunch();
    void exeLaunchEnd();

private:
    al::DeriveActorGroup<GiantWanderBossMine>* mMineGroup = nullptr;
    GiantWanderBossMine* mMine = nullptr;
    s32 mLaunchCount = 0;
};

static_assert(sizeof(GiantWanderBossStateEscapeCancel) == 0x38);
