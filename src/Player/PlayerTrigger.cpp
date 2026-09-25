#include "Player/PlayerTrigger.h"

PlayerTrigger::PlayerTrigger() = default;

void PlayerTrigger::set(ECollisionTrigger flag) {
    mCollisionTrigger.setBit(flag);
}

void PlayerTrigger::set(EAttackSensorTrigger flag) {
    mAttackSensorTrigger.setBit(flag);
}

void PlayerTrigger::set(EActionTrigger flag) {
    mActionTrigger.setBit(flag);
}

void PlayerTrigger::set(EReceiveSensorTrigger flag) {
    mReceiveSensorTrigger.setBit(flag);
}

void PlayerTrigger::set(EPreMovementTrigger flag) {
    mPreMovementTrigger.setBit(flag);
}

void PlayerTrigger::set(EDemoEndTrigger flag) {
    mDemoEndTrigger.setBit(flag);
}

void PlayerTrigger::set(EMaterialChangeTrigger flag) {
    mMaterialChangeTrigger.setBit(flag);
}

void PlayerTrigger::setRecMaterialTrigger(const char* materialTrigger) {
    mRecMaterialTrigger = materialTrigger;
}

void PlayerTrigger::clearCollisionTrigger() {
    mCollisionTrigger = 0;
}

void PlayerTrigger::clearAttackSensorTrigger() {
    mAttackSensorTrigger = 0;
}

void PlayerTrigger::clearActionTrigger() {
    mActionTrigger = 0;
}

void PlayerTrigger::clearReceiveSensorTrigger() {
    mReceiveSensorTrigger = 0;
    mRecMaterialTrigger = nullptr;
}

void PlayerTrigger::clearPreMovementTrigger() {
    mPreMovementTrigger = 0;
}

void PlayerTrigger::clearDemoEndTrigger() {
    mDemoEndTrigger = 0;
}

void PlayerTrigger::clearMaterialChangeTrigger() {
    mMaterialChangeTrigger = 0;
}

bool PlayerTrigger::isOn(ECollisionTrigger flag) const {
    return mCollisionTrigger.isOnBit(flag);
}

bool PlayerTrigger::isOn(EAttackSensorTrigger flag) const {
    return mAttackSensorTrigger.isOnBit(flag);
}

bool PlayerTrigger::isOn(EActionTrigger flag) const {
    return mActionTrigger.isOnBit(flag);
}

bool PlayerTrigger::isOn(EReceiveSensorTrigger flag) const {
    return mReceiveSensorTrigger.isOnBit(flag);
}

bool PlayerTrigger::isOn(EPreMovementTrigger flag) const {
    return mPreMovementTrigger.isOnBit(flag);
}

bool PlayerTrigger::isOn(EDemoEndTrigger flag) const {
    return mDemoEndTrigger.isOnBit(flag);
}

bool PlayerTrigger::isOn(EMaterialChangeTrigger flag) const {
    return mMaterialChangeTrigger.isOnBit(flag);
}

bool PlayerTrigger::isOnUpperPunchHit() const {
    return mCollisionTrigger.isOnBit(ECollisionTrigger_val0) || isOnUpperPunchHitToss();
}

bool PlayerTrigger::isOnUpperPunchHitToss() const {
    return mAttackSensorTrigger.isOnBit(EAttackSensorTrigger_val4);
}

bool PlayerTrigger::isOnAnyDamage() const {
    return isOn(ECollisionTrigger_val2) || isOn(ECollisionTrigger_val6) ||
           isOn(EReceiveSensorTrigger_val0) || isOn(EActionTrigger_val23) || isOnDamageFire() ||
           isOnEndHackWithDamage();
}

bool PlayerTrigger::isOnDamageFire() const {
    if (mCollisionTrigger.isOnBit(ECollisionTrigger_val3) ||
        mCollisionTrigger.isOnBit(ECollisionTrigger_val4))
        return true;
    return mReceiveSensorTrigger.isOnBit(EReceiveSensorTrigger_val1);
}

bool PlayerTrigger::isOnEndHackWithDamage() const {
    return mReceiveSensorTrigger.isOnBit(EReceiveSensorTrigger_val2) ||
           mReceiveSensorTrigger.isOnBit(EReceiveSensorTrigger_val3);
}

bool PlayerTrigger::isOnNoDamageDown() const {
    if (mActionTrigger.isOnBit(EActionTrigger_val10))
        return true;
    return mPreMovementTrigger.isOnBit(EPreMovementTrigger_val4);
}

bool PlayerTrigger::isOnSpinMoveCapThrow() const {
    return mActionTrigger.isOnBit(EActionTrigger_val27) ||
           mActionTrigger.isOnBit(EActionTrigger_val28);
}

bool PlayerTrigger::isOnHipDropCancelThrow() const {
    return mActionTrigger.isOnBit(EActionTrigger_val18);
}

bool PlayerTrigger::isOnYoshiHackEnd() const {
    return mCollisionTrigger.isOnBit(ECollisionTrigger_val10);
}

bool PlayerTrigger::isOnCollisionExpandCheck() const {
    return mActionTrigger.isOnBit(EActionTrigger_val3);
}

bool PlayerTrigger::tryGetRecMaterialCode(const char** dest) const {
    if (!mRecMaterialTrigger)
        return false;
    *dest = mRecMaterialTrigger;
    return true;
}
