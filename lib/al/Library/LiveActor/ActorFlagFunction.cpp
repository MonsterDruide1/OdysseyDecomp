#include "Library/LiveActor/ActorFlagFunction.h"

#include "Library/Collision/Collider.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorFlag.h"
#include "Library/Shadow/ShadowKeeper.h"
#include "Library/Shadow/ShadowMaskCtrl.h"

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
    actor->getFlags()->isDisableCalcAnim = false;
}

void offCalcAnim(LiveActor* actor) {
    actor->getFlags()->isDisableCalcAnim = true;
}

void validateShadow(LiveActor* actor) {
    ShadowMaskCtrl* shadowMaskCtrl = actor->getShadowKeeper()->getShadowMaskCtrl();
    if (shadowMaskCtrl)
        shadowMaskCtrl->validate();
}

void invalidateShadow(LiveActor* actor) {
    ShadowMaskCtrl* shadowMaskCtrl = actor->getShadowKeeper()->getShadowMaskCtrl();
    if (shadowMaskCtrl)
        shadowMaskCtrl->invalidate();
}

void onCollide(LiveActor* actor) {
    actor->getFlags()->isCollideOff = false;
    Collider* collider = actor->getCollider();
    if (collider)
        collider->onInvalidate();
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
    actor->getFlags()->isAreaTargetOn = false;
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
