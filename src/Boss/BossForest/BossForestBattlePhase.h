#pragma once

#include <math/seadQuat.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct ActorInitInfo;
struct ActorParamS32;
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al

class BossForest;
class BossForestSatelliteController;
class BossForestStateKnockOver;
class BossForestStateAttack;
class BossForestStateRingBeamAttack;
class BossForestStateRecovery;

class BossForestBattlePhase : public al::HostStateBase<BossForest> {
public:
    BossForestBattlePhase(BossForest* boss, const al::ActorInitInfo& initInfo, s32 phaseIndex);
    ~BossForestBattlePhase() override;

    void appear() override;
    bool isStateAttackable() const;
    void attackSensor(al::HitSensor* self, al::HitSensor* other);
    bool receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self);
    bool tryAttack();
    void updateEyeJoint();

    BossForestSatelliteController* getSatelliteController() const { return mSatelliteController; }

    void exeWait();
    void exeAttack();
    void exeRingBeamAttack();
    void exeKnockOver();
    void exeRecover();
    void exeReactionCoreBreak();
    void exeLastDamage();
    bool tryTransitionCoreBreak();

private:
    s32 mPhaseIndex = 0;
    al::LiveActor* mPhaseActor = nullptr;
    al::ActorParamS32** mBeamParams = nullptr;
    BossForestStateKnockOver* mStateKnockOver = nullptr;
    BossForestStateAttack* mStateAttack = nullptr;
    BossForestStateRingBeamAttack* mStateRingBeamAttack = nullptr;
    BossForestStateRecovery* mStateRecovery = nullptr;
    BossForestSatelliteController* mSatelliteController = nullptr;
    bool _60 = false;
    bool mHasAttacked = false;
    sead::Quatf mQuat = sead::Quatf::unit;
};

static_assert(sizeof(BossForestBattlePhase) == 0x78);
