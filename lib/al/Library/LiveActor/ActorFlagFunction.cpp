#include "Library/LiveActor/ActorFlagFunction.h"

#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorFlag.h"

namespace al {
bool isAlive(const LiveActor* actor) {
    return !actor->getFlags()->isDead;
}

bool isDead(const LiveActor* actor) {
    return actor->getFlags()->isDead;
}

bool isNoCollide(const LiveActor* actor) {
    return actor->getFlags()->isCollideOff;
}

void onCalcAnim(LiveActor* actor) {
    actor->getFlags()->isCalcAnim = true;
}

void offCalcAnim(LiveActor* actor) {
    actor->getFlags()->isCalcAnim = false;
}

void validateShadow(LiveActor* actor) {}

void invalidateShadow(LiveActor* actor) {}

void onCollide(LiveActor* actor) {
    actor->getFlags()->isCollideOff = false;
}

void offCollide(LiveActor* actor) {
    actor->getFlags()->isCollideOff = true;
}

void validateMaterialCode(LiveActor* actor) {
    actor->getFlags()->isMaterialCodeValid = true;
}

void invalidateMaterialCode(LiveActor* actor) {
    actor->getFlags()->isMaterialCodeValid = false;
}

void validatePuddleMaterial(LiveActor* actor) {
    actor->getFlags()->isPuddleMaterialValid = true;
}

void invalidatePuddleMaterial(LiveActor* actor) {
    actor->getFlags()->isPuddleMaterialValid = false;
}

bool isAreaTarget(const LiveActor* actor) {
    return actor->getFlags()->isAreaTargetOn;
}

void onAreaTarget(LiveActor* actor) {
    actor->getFlags()->isAreaTargetOn = true;
}

void offAreaTarget(LiveActor* actor) {
    actor->getFlags()->isAreaTargetOn = true;
}

bool isUpdateMovementEffectAudioCollisionSensor(const LiveActor* actor) {
    return actor->getFlags()->isUpdateMovementEffectAudioCollisionSensor;
}

void onUpdateMovementEffectAudioCollisionSensor(LiveActor* actor) {
    actor->getFlags()->isUpdateMovementEffectAudioCollisionSensor = true;
}

void offUpdateMovementEffectAudioCollisionSensor(LiveActor* actor) {
    actor->getFlags()->isUpdateMovementEffectAudioCollisionSensor = false;
}
}  // namespace al
