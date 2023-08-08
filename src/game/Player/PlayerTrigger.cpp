#include "game/Player/PlayerTrigger.h"

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

bool PlayerTrigger::tryGetRecMaterialCode(const char** dest) const {
    if (!mRecMaterialTrigger)
        return false;
    *dest = mRecMaterialTrigger;
    return true;
}
