#pragma once

namespace al {
class LiveActor;

bool isAlive(const LiveActor* actor);
bool isDead(const LiveActor* actor);
bool isNoCollide(const LiveActor* actor);
void onCalcAnim(LiveActor* actor);
void offCalcAnim(LiveActor* actor);
void validateShadow(LiveActor* actor);
void invalidateShadow(LiveActor* actor);
void onCollide(LiveActor* actor);
void offCollide(LiveActor* actor);
void validateMaterialCode(LiveActor* actor);
void invalidateMaterialCode(LiveActor* actor);
void validatePuddleMaterial(LiveActor* actor);
void invalidatePuddleMaterial(LiveActor* actor);
bool isAreaTarget(const LiveActor* actor);
void onAreaTarget(LiveActor* actor);
void offAreaTarget(LiveActor* actor);
bool isUpdateMovementEffectAudioCollisionSensor(const LiveActor* actor);
void onUpdateMovementEffectAudioCollisionSensor(LiveActor* actor);
void offUpdateMovementEffectAudioCollisionSensor(LiveActor* actor);
}  // namespace al
