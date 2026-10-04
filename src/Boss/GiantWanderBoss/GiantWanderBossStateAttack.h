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
    enum class MineAttackType : s32 {
        None = 0,
        FirstPhase = 1,
        Escape = 2,
        LongRange = 3,
    };

    void startAttackAction(const char* actionName);

    GiantWanderBossBullet* mBullet = nullptr;
    GiantWanderBossMine* mMine = nullptr;
    MineAttackType mMineAttackType = MineAttackType::None;
};

static_assert(sizeof(GiantWanderBossStateAttack) == 0x38);
