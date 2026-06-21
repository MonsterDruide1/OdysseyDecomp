#pragma once

#include "Library/LiveActor/LiveActor.h"

class BombTailCap : public al::LiveActor {
public:
    BombTailCap(al::LiveActor* parent_actor);
    void initCap(al::LiveActor* parent_actor, const al::ActorInitInfo& info);
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void appear() override;
    void disappear();
    void startBattle();
    void startPanicEnd();
    void startResetAttack();
    void endResetAttack();
    void startReaction();
    void startReactionWithoutAnim(const al::SensorMsg* message, al::HitSensor* other,
                                  al::HitSensor* self);
    void exeAppearPanicEnd();
    void exeBeforeBattleStart();
    void exeWait();
    void exeBlowDown();
    void exeDisappear();
    void exeReaction();
    void exeResetAttackState();

private:
    char filler[0x58];
};

static_assert(sizeof(BombTailCap) == 0x160);
