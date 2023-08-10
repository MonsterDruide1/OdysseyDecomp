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
void onCalcAnim(al::LiveActor* actor) {
    actor->getFlags()->isCalcAnim = true;
}
void offCalcAnim(al::LiveActor* actor) {
    actor->getFlags()->isCalcAnim = false;
}
void validateShadow(LiveActor* actor) {}
void invalidateShadow(LiveActor* actor) {}
void onCollide(LiveActor* actor) {
    actor->getFlags()->isCollideOff = false;
}
void offCollide(al::LiveActor* actor) {
    actor->getFlags()->isCollideOff = true;
}
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
