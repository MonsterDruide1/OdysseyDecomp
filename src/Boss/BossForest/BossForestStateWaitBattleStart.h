#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class BossForest;
class CapTargetInfo;

class BossForestStateWaitBattleStart : public al::HostStateBase<BossForest> {
public:
    BossForestStateWaitBattleStart(BossForest* boss, const al::ActorInitInfo& initInfo);

    void appear() override;
    void kill() override;

    bool receiveMsg(const al::SensorMsg* msg, al::HitSensor* self, al::HitSensor* other);

    void exeWait();
    void exeCapKeepOn();
    void endCapKeepOn();
    void exeDamage();
    void exeKnockOver();
    void exeDamageEnd();

private:
    CapTargetInfo* mCapTargetInfo = nullptr;
    s32 mDamageCount = 0;
    s32 mInvincibleTimer = 0;
    bool mHasItemDropped = false;
};

static_assert(sizeof(BossForestStateWaitBattleStart) == 0x38);
