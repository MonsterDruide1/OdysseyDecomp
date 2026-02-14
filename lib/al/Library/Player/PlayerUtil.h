#pragma once

#include <math/seadVector.h>

namespace al {
class HitSensor;
class LiveActor;
class PadRumbleKeeper;
class PlayerHolder;

s32 getPlayerNumMax(const LiveActor* actor);
s32 getPlayerNumMax(const PlayerHolder* holder);
s32 getAlivePlayerNum(const LiveActor* actor);
s32 getAlivePlayerNum(const PlayerHolder* holder);
LiveActor* getPlayerActor(const LiveActor* actor, s32 index);
LiveActor* getPlayerActor(const PlayerHolder* holder, s32 index);
const sead::Vector3f& getPlayerPos(const LiveActor* actor, s32 index);
const sead::Vector3f& getPlayerPos(const PlayerHolder* holder, s32 index);
LiveActor* tryGetPlayerActor(const LiveActor* actor, s32 index);
LiveActor* tryGetPlayerActor(const PlayerHolder* holder, s32 index);
bool isPlayerDead(const LiveActor* actor, s32 index);
bool isPlayerDead(const PlayerHolder* holder, s32 index);
bool isPlayerAreaTarget(const LiveActor* actor, s32 index);
bool isPlayerAreaTarget(const PlayerHolder* holder, s32 index);
LiveActor* tryFindAlivePlayerActorFirst(const LiveActor* actor);
LiveActor* tryFindAlivePlayerActorFirst(const PlayerHolder* holder);
LiveActor* findAlivePlayerActorFirst(const LiveActor* actor);
LiveActor* findAlivePlayerActorFirst(const PlayerHolder* holder);
PadRumbleKeeper* getPlayerPadRumbleKeeper(const LiveActor* actor, s32 index);
s32 getPlayerPort(const PlayerHolder* holder, s32 index);
s32 getPlayerPort(const LiveActor* actor, s32 index);
LiveActor* findAlivePlayerActorFromPort(const PlayerHolder* holder, s32 port);
LiveActor* tryFindAlivePlayerActorFromPort(const PlayerHolder* holder, s32 port);
LiveActor* findAlivePlayerActorFromPort(const LiveActor* actor, s32 port);
LiveActor* tryFindAlivePlayerActorFromPort(const LiveActor* actor, s32 port);
s32 findNearestPlayerId(const LiveActor* actor, f32 threshold);
LiveActor* findNearestPlayerActor(const LiveActor* actor);
LiveActor* tryFindNearestPlayerActor(const LiveActor* actor);
const sead::Vector3f& findNearestPlayerPos(const LiveActor* actor);
bool tryFindNearestPlayerPos(sead::Vector3f* pos, const LiveActor* actor);
bool tryFindNearestPlayerDisatanceFromTarget(f32* distance, const LiveActor* actor,
                                             const sead::Vector3f& target);
bool isNearPlayer(const LiveActor* actor, f32 threshold);
bool isNearPlayerH(const LiveActor* actor, f32 threshold);
bool isNearPlayerHCondition(const LiveActor* actor, f32 threshold,
                            bool (*condition)(const LiveActor*));
const sead::Vector3f& getFarPlayerPosMaxX(const LiveActor* actor);
const sead::Vector3f& getFarPlayerPosMinX(const LiveActor* actor);
u32 calcPlayerListOrderByDistance(const LiveActor* actor, const LiveActor** actorList, u32 size);
u32 calcAlivePlayerActor(const LiveActor* actor, const LiveActor** actorList, u32 size);
LiveActor* tryFindNearestPlayerActorCondition(const LiveActor* actor,
                                              bool (*condition)(const LiveActor*));
bool tryFindNearestPlayerPosCondition(sead::Vector3f* pos, const LiveActor* actor,
                                      bool (*condition)(const LiveActor*));
bool isResetablePlayerPos(const LiveActor* actor, const sead::Vector3f& pos, f32 clippingRadius,
                          f32 threshold);
bool isResetablePlayerPos(const LiveActor* actor, f32 threshold);
void faceToPlayer(LiveActor* actor);
}  // namespace al

namespace alPlayerFunction {
void registerPlayer(al::LiveActor* actor, al::PadRumbleKeeper* padRumbleKeeper);
bool isFullPlayerHolder(al::LiveActor* actor);
s32 findPlayerHolderIndex(const al::LiveActor* actor);
s32 findPlayerHolderIndex(const al::HitSensor* sensor);
bool isPlayerActor(const al::LiveActor* actor);
bool isPlayerActor(const al::HitSensor* sensor);
}  // namespace alPlayerFunction
