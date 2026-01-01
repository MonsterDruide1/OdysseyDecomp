#pragma once

#include <basis/seadTypes.h>
#include <prim/seadBitFlag.h>

class PlayerTrigger {
public:
    enum ECollisionTrigger : u32 {
        // used in PlayerStateHipDrop
        ECollisionTrigger_val1 = 1,
        // used in PlayerStateDamageLife
        ECollisionTrigger_val6 = 6,
        // used in PlayerJudgeWallHitDown
        ECollisionTrigger_val9 = 9,
	// used in PlayerTrigger
	ECollisionTrigger_val1024 = 1024,
    };

    enum EAttackSensorTrigger : u32 {
        // used in PlayerCounterAfterCapCatch
        EAttackSensorTrigger_val0 = 0,
    };

    enum EActionTrigger : u32 {
        // used in PlayerStateHipDrop
        EActionTrigger_val3 = 3,
        // used in PlayerStateDamageLife
        EActionTrigger_val6 = 6,
        // used in PlayerJudgeForceLand
        EActionTrigger_val11 = 11,
        // used in PlayerStateSquat
        EActionTrigger_val12 = 12,
        // used in PlayerJudgeWallCatch, PlayerJudgeWallKeep
        EActionTrigger_val30 = 30,
        EActionTrigger_QuickTurn = 34,
    };

    enum EReceiveSensorTrigger : u32 {};

    enum EPreMovementTrigger : u32 {
        // used in PlayerStateDamageLife
        EPreMovementTrigger_val2 = 2,
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
