#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
class HitSensor;
class Nerve;

void startAction(LiveActor* actor, const char* actionName);
void startActionAtRandomFrame(LiveActor* actor, const char* actionName);
bool tryStartAction(LiveActor* actor, const char* actionName);
bool tryStartActionIfNotPlaying(LiveActor* actor, const char* actionName);
bool tryStartActionIfActionEnd(LiveActor* actor, const char* actionName);

bool isActionPlaying(LiveActor* actor, const char* actionName);
bool isActionEnd(LiveActor* actor);
bool isExistAction(LiveActor* actor);
bool isExistAction(LiveActor* actor, const char* actionName);
bool isActionOneTime(LiveActor* actor);
bool isActionOneTime(LiveActor* actor, const char* actionName);

const char* getActionName(LiveActor* actor);
f32 getActionFrame(LiveActor* actor);
f32 getActionFrameMax(LiveActor* actor);
f32 getActionFrameMax(LiveActor* actor, const char* actionName);
f32 getActionFrameRate(LiveActor* actor);

void setActionFrame(LiveActor* actor, f32 frame);
void setActionFrameRate(LiveActor* actor, f32 rate);

void stopAction(LiveActor* actor);
void restartAction(LiveActor* actor);
void copyAction(LiveActor* actor, const LiveActor* sourceActor);

void startNerveAction(LiveActor* actor, const char* actionName);
void setNerveAtActionEnd(LiveActor* actor, const Nerve* nerve);
void resetNerveActionForInit(LiveActor* actor);

void startHitReaction(const LiveActor*, const char*);
void startHitReactionHitEffect(const LiveActor*, const char*, const HitSensor*, const HitSensor*);
void startHitReactionHitEffect(const LiveActor*, const char*, const sead::Vector3f&);
void startHitReactionHitEffect(const LiveActor*, const char*, const sead::Matrix34f*);
void startHitReactionBlowHit(const LiveActor*, const HitSensor*, const HitSensor*);
void startHitReactionBlowHit(const LiveActor*, const sead::Vector3f&);
void startHitReactionBlowHit(const LiveActor*);
void startHitReactionBlowHitDirect(const LiveActor*, const HitSensor*, const HitSensor*);
void startHitReactionBlowHitDirect(const LiveActor*, const sead::Vector3f&);
void startHitReactionBlowHitDirect(const LiveActor*);

}  // namespace al
