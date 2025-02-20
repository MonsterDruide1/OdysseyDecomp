#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class ActorActionKeeper;
class HitSensor;
class LiveActor;
class Nerve;

void startAction(LiveActor* actor, const char* actionName);
s32 startActionAtRandomFrame(LiveActor* actor, const char* actionName);
bool tryStartAction(LiveActor* actor, const char* actionName);
bool tryStartActionIfNotPlaying(LiveActor* actor, const char* actionName);
bool tryStartActionIfActionEnd(LiveActor* actor, const char* actionName);

bool isActionPlaying(const LiveActor* actor, const char* actionName);
bool isActionEnd(const LiveActor* actor);
bool isExistAction(const LiveActor* actor);
bool isExistAction(const LiveActor* actor, const char* actionName);
bool isActionOneTime(const LiveActor* actor);
bool isActionOneTime(const LiveActor* actor, const char* actionName);

const char* getActionName(const LiveActor* actor);
f32 getActionFrame(const LiveActor* actor);
f32 getActionFrameMax(const LiveActor* actor);
f32 getActionFrameMax(const LiveActor* actor, const char* actionName);
f32 getActionFrameRate(const LiveActor* actor);

void setActionFrame(LiveActor* actor, f32 frame);
void setActionFrameRate(LiveActor* actor, f32 rate);

void stopAction(LiveActor* actor);
void restartAction(LiveActor* actor);
void copyAction(LiveActor* actor, const LiveActor* sourceActor);

void startNerveAction(LiveActor* actor, const char* actionName);
void setNerveAtActionEnd(LiveActor* actor, const Nerve* nerve);
void resetNerveActionForInit(LiveActor* actor);

void startHitReaction(const LiveActor* actor, const char* name);
void startHitReactionHitEffect(const LiveActor* actor, const char* name, const HitSensor* other,
                               const HitSensor* self);
void startHitReactionHitEffect(const LiveActor* actor, const char* name, const sead::Vector3f& pos);
void startHitReactionHitEffect(const LiveActor* actor, const char* name,
                               const sead::Matrix34f* mtx);
void startHitReactionBlowHit(const LiveActor* actor, const HitSensor* other, const HitSensor* self);
void startHitReactionBlowHit(const LiveActor* actor, const sead::Vector3f& pos);
void startHitReactionBlowHit(const LiveActor* actor);
void startHitReactionBlowHitDirect(const LiveActor* actor, const HitSensor* other,
                                   const HitSensor* self);
void startHitReactionBlowHitDirect(const LiveActor* actor, const sead::Vector3f& pos);
void startHitReactionBlowHitDirect(const LiveActor* actor);

}  // namespace al
