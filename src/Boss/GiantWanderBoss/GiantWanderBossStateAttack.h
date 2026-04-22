#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

class GiantWanderBossBullet;
class GiantWanderBossMine;

class GiantWanderBossStateAttack : public al::ActorStateBase {
public:
    GiantWanderBossStateAttack(al::LiveActor* actor);

    void appear() override;
    void kill() override;
    void startWithBullet(GiantWanderBossBullet* bullet);
    void startWithMineFirstPhase(GiantWanderBossMine* mine);
    void startWithMineEscape(GiantWanderBossMine* mine);
    void startWithMineLongRange(GiantWanderBossMine* mine);
    void exeAttackSign();
    void exeAttackSignWait();
    void exeAttackStart();
    void exeAttack();
    void exeAttackEnd();

    al::LiveActor* getActor() const { return mActor; }

private:
    GiantWanderBossBullet* mBullet = nullptr;
    GiantWanderBossMine* mMine = nullptr;
    s32 mMineAttackType = 0;
};

static_assert(sizeof(GiantWanderBossStateAttack) == 0x38);
