#pragma once

#include <basis/seadTypes.h>
#include <prim/seadBitFlag.h>

class PlayerTrigger {
public:
    enum ECollisionTrigger : u32 {
        // used in PlayerTrigger::isOnUpperPunchHit
        ECollisionTrigger_val0 = 0,
        // used in PlayerStateHipDrop::isLandTrigger
        ECollisionTrigger_val1 = 1,
        // used in PlayerTrigger::isOnAnyDamage
        ECollisionTrigger_val2 = 2,
        // used in PlayerTrigger::isOnDamageFire
        ECollisionTrigger_val3 = 3,
        // used in PlayerTrigger::isOnDamageFire
        ECollisionTrigger_val4 = 4,
        // used in PlayerStateDamageLife::exeDead, PlayerTrigger::isOnAnyDamage
        ECollisionTrigger_val6 = 6,
        // used in PlayerJudgeWallHitDown::judge
        ECollisionTrigger_val9 = 9,
        // used in PlayerTrigger::isOnYoshiHackEnd
        ECollisionTrigger_val10 = 10,
    };

    enum EAttackSensorTrigger : u32 {
        // used in PlayerCounterAfterCapCatch::isCapCatch
        EAttackSensorTrigger_val0 = 0,
        // used in PlayerTrigger::isOnUpperPunchHitToss
        EAttackSensorTrigger_val4 = 4,
    };

    enum EActionTrigger : u32 {
        // used in PlayerStateHipDrop::exeStart
        EActionTrigger_val3 = 3,
        // used in PlayerStateDamageLife::appear
        EActionTrigger_val6 = 6,
        // used in PlayerTrigger::isOnNoDamageDown
        EActionTrigger_val10 = 10,
        // used in PlayerJudgeForceLand::judge
        EActionTrigger_val11 = 11,
        // used in PlayerStateSquat::appear
        EActionTrigger_val12 = 12,
        // used in PlayerTrigger::isOnHipDropCancelThrow
        EActionTrigger_val18 = 18,
        // used in PlayerTrigger::isOnAnyDamage
        EActionTrigger_val23 = 23,
        // used in PlayerTrigger::isOnSpinMoveCapThrow
        EActionTrigger_val27 = 27,
        EActionTrigger_val28 = 28,
        // used in PlayerJudgeWallCatch::update, PlayerJudgeWallKeep::update
        EActionTrigger_val30 = 30,
        // used in PlayerCounterQuickTurnJump::isEnableTurnJump
        EActionTrigger_QuickTurn = 34,
    };

    enum EReceiveSensorTrigger : u32 {
        // used in PlayerTrigger::isOnAnyDamage
        EReceiveSensorTrigger_val0 = 0,
        // used in PlayerTrigger::isOnDamageFire
        EReceiveSensorTrigger_val1 = 1,
        // used in PlayerTrigger::isOnEndHackWithDamage
        EReceiveSensorTrigger_val2 = 2,
        // used in PlayerTrigger::isOnEndHackWithDamage
        EReceiveSensorTrigger_val3 = 3,
    };

    enum EPreMovementTrigger : u32 {
        // used in PlayerStateDamageLife::appear
        EPreMovementTrigger_val2 = 2,
        // used in PlayerTrigger::isOnNoDamageDown
        EPreMovementTrigger_val4 = 4,
    };

    enum EDemoEndTrigger : u32 {};

    enum EMaterialChangeTrigger : u32 {};

    PlayerTrigger();
    void set(ECollisionTrigger flag);
    void set(EAttackSensorTrigger flag);
    void set(EActionTrigger flag);
    void set(EReceiveSensorTrigger flag);
    void set(EPreMovementTrigger flag);
    void set(EDemoEndTrigger flag);
    void set(EMaterialChangeTrigger flag);
    void setRecMaterialTrigger(const char* materialTrigger);
    void clearCollisionTrigger();
    void clearAttackSensorTrigger();
    void clearActionTrigger();
    void clearReceiveSensorTrigger();
    void clearPreMovementTrigger();
    void clearDemoEndTrigger();
    void clearMaterialChangeTrigger();
    bool isOn(ECollisionTrigger flag) const;
    bool isOn(EAttackSensorTrigger flag) const;
    bool isOn(EActionTrigger flag) const;
    bool isOn(EReceiveSensorTrigger flag) const;
    bool isOn(EPreMovementTrigger flag) const;
    bool isOn(EDemoEndTrigger flag) const;
    bool isOn(EMaterialChangeTrigger flag) const;
    bool isOnUpperPunchHit() const;
    bool isOnUpperPunchHitToss() const;
    bool isOnAnyDamage() const;
    bool isOnDamageFire() const;
    bool isOnEndHackWithDamage() const;
    bool isOnNoDamageDown() const;
    bool isOnSpinMoveCapThrow() const;
    bool isOnHipDropCancelThrow() const;
    bool isOnYoshiHackEnd() const;
    bool isOnCollisionExpandCheck() const;
    bool tryGetRecMaterialCode(const char** dest) const;

private:
    sead::BitFlag32 mCollisionTrigger = 0;
    sead::BitFlag32 mAttackSensorTrigger;
    sead::BitFlag64 mActionTrigger = 0;
    sead::BitFlag32 mReceiveSensorTrigger = 0;
    sead::BitFlag32 mPreMovementTrigger = 0;
    sead::BitFlag32 mDemoEndTrigger = 0;
    sead::BitFlag32 mMaterialChangeTrigger = 0;
    const char* mRecMaterialTrigger = nullptr;
};

static_assert(sizeof(PlayerTrigger) == 0x28);
