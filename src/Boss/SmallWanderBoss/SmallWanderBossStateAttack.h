#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

class SmallWanderBossBullet;

class SmallWanderBossStateAttack : public al::ActorStateBase {
public:
    SmallWanderBossStateAttack(al::LiveActor* actor);

    void appear() override;
    void kill() override;

    void startWithBullet(SmallWanderBossBullet* bullet);
    void exeAttackSign();
    void exeAttackSignWait();
    void exeAttackStart();
    void exeAttack();
    void exeAttackEnd();

private:
    SmallWanderBossBullet* mBullet;
};

static_assert(sizeof(SmallWanderBossStateAttack) == 0x28);
