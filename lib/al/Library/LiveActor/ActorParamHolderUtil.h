#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ActorParamF32;
struct ActorParamJump;
struct ActorParamMove;
struct ActorParamRebound;
struct ActorParamS32;
struct ActorParamSight;
class LiveActor;

ActorParamF32* findActorParamF32(const LiveActor* actor, const char* paramName);
ActorParamS32* findActorParamS32(const LiveActor* actor, const char* paramName);
ActorParamMove* findActorParamMove(const LiveActor* actor, const char* paramName);
ActorParamJump* findActorParamJump(const LiveActor* actor, const char* paramName);
ActorParamSight* findActorParamSight(const LiveActor* actor, const char* paramName);
ActorParamRebound* findActorParamRebound(const LiveActor* actor, const char* paramName);
void setActorParamMove(ActorParamMove* paramMove, f32 moveAccel, f32 gravity, f32 moveFriction,
                       f32 turnSpeedDegree);

}  // namespace al
