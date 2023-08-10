#include "al/Library/LiveActor/ActorFlagFunction.h"
#include "al/Library/LiveActor/LiveActor.h"

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
void onCalcAnim(LiveActor* actor) {}
void offCalcAnim(LiveActor* actor) {}
void validateShadow(LiveActor* actor) {}
void invalidateShadow(LiveActor* actor) {}
void onCollide(LiveActor* actor) {
    actor->getFlags()->isCollideOff = false;
}
void offCollide(LiveActor* actor) {}
void validateMaterialCode(LiveActor* actor) {}
void invalidateMaterialCode(LiveActor* actor) {}
void validatePuddleMaterial(LiveActor* actor) {}
void invalidatePuddleMaterial(LiveActor* actor) {}
bool isAreaTarget(const LiveActor* actor) {}
void onAreaTarget(LiveActor* actor) {}
void offAreaTarget(LiveActor* actor) {}
void isUpdateMovementEffectAudioCollisionSensor(const LiveActor* actor) {}
void onUpdateMovementEffectAudioCollisionSensor(LiveActor* actor) {}
void offUpdateMovementEffectAudioCollisionSensor(LiveActor* actor) {}
}
