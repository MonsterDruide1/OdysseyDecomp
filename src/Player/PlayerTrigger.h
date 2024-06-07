#pragma once

#include <basis/seadTypes.h>
#include <prim/seadBitFlag.h>

class PlayerTrigger {
public:
    enum ECollisionTrigger : u32 {
        ECollisionTrigger_val7 = 7,
        ECollisionTrigger_val8 = 8,
    };
    enum EAttackSensorTrigger : u32 {
        // used in PlayerCounterAfterCapCatch
        EAttackSensorTrigger_val0 = 0,
    };
    enum EActionTrigger : u32 {
        EActionTrigger_val1 = 1,
        EActionTrigger_val2 = 2,
        EActionTrigger_val8 = 8,
        EActionTrigger_val9 = 9,
        EActionTrigger_val10 = 10,
        EActionTrigger_val11 = 11,
        EActionTrigger_val14 = 14,
        EActionTrigger_val17 = 17,
        EActionTrigger_val18 = 18,
        EActionTrigger_val19 = 19,
        EActionTrigger_val21 = 21,
        EActionTrigger_val24 = 24,
        EActionTrigger_val27 = 27,
        EActionTrigger_val28 = 28,
        EActionTrigger_val30 = 30,
        EActionTrigger_val32 = 32,
        EActionTrigger_val33 = 33,
        EActionTrigger_QuickTurn = 34,
    };
    enum EReceiveSensorTrigger : u32 {};
    enum EPreMovementTrigger : u32 {};
    enum EDemoEndTrigger : u32 {};
    enum EMaterialChangeTrigger : u32 {
        EMaterialChangeTrigger_val0 = 0,
    };

    PlayerTrigger();
    void set(ECollisionTrigger);
    void set(EAttackSensorTrigger);
    void set(EActionTrigger);
    void set(EReceiveSensorTrigger);
    void set(EPreMovementTrigger);
    void set(EDemoEndTrigger);
    void set(EMaterialChangeTrigger);
    void setRecMaterialTrigger(const char*);
    void clearCollisionTrigger();
    void clearAttackSensorTrigger();
    void clearActionTrigger();
    void clearReceiveSensorTrigger();
    void clearPreMovementTrigger();
    void clearDemoEndTrigger();
    void clearMaterialChangeTrigger();
    bool isOn(ECollisionTrigger) const;
    bool isOn(EAttackSensorTrigger) const;
    bool isOn(EActionTrigger) const;
    bool isOn(EReceiveSensorTrigger) const;
    bool isOn(EPreMovementTrigger) const;
    bool isOn(EDemoEndTrigger) const;
    bool isOn(EMaterialChangeTrigger) const;
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
    bool tryGetRecMaterialCode(const char**) const;

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
