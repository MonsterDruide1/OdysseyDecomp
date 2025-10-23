#pragma once

#include <math/seadVector.h>

namespace al {
class HitSensor;
class LiveActor;
class PadRumbleKeeper;
class PlayerHolder;

s32 getPlayerNumMax(const LiveActor*);
s32 getPlayerNumMax(const PlayerHolder* holder);
s32 getAlivePlayerNum(const LiveActor*);
s32 getAlivePlayerNum(const PlayerHolder* holder);
LiveActor* getPlayerActor(const LiveActor*, s32);
LiveActor* getPlayerActor(const PlayerHolder* holder, s32 index);
const sead::Vector3f& getPlayerPos(const LiveActor*, s32);
const sead::Vector3f& getPlayerPos(const PlayerHolder* holder, s32 index);
LiveActor* tryGetPlayerActor(const LiveActor*, s32);
LiveActor* tryGetPlayerActor(const PlayerHolder* holder, s32 index);
bool isPlayerDead(const LiveActor*, s32);
bool isPlayerDead(const PlayerHolder* holder, s32 index);
bool isPlayerAreaTarget(const LiveActor*, s32);
bool isPlayerAreaTarget(const PlayerHolder* holder, s32 index);
LiveActor* tryFindAlivePlayerActorFirst(const LiveActor*);
LiveActor* tryFindAlivePlayerActorFirst(const PlayerHolder* holder);
LiveActor* findAlivePlayerActorFirst(const LiveActor*);
LiveActor* findAlivePlayerActorFirst(const PlayerHolder* holder);
PadRumbleKeeper* getPlayerPadRumbleKeeper(const LiveActor*, s32);
s32 getPlayerPort(const PlayerHolder*, s32);
s32 getPlayerPort(const LiveActor*, s32);
LiveActor* findAlivePlayerActorFromPort(const PlayerHolder*, s32);
LiveActor* tryFindAlivePlayerActorFromPort(const PlayerHolder*, s32);
LiveActor* findAlivePlayerActorFromPort(const LiveActor*, s32);
LiveActor* tryFindAlivePlayerActorFromPort(const LiveActor*, s32);
s32 findNearestPlayerId(const LiveActor*, f32);
LiveActor* findNearestPlayerActor(const LiveActor*);
LiveActor* tryFindNearestPlayerActor(const LiveActor*);
const sead::Vector3f& findNearestPlayerPos(const LiveActor*);
bool tryFindNearestPlayerPos(sead::Vector3f*, const LiveActor*);
bool tryFindNearestPlayerDisatanceFromTarget(f32*, const LiveActor*, const sead::Vector3f&);
bool isNearPlayer(const LiveActor*, f32);
bool isNearPlayerH(const LiveActor*, f32);
bool isNearPlayerHCondition(const LiveActor*, f32, bool (*)(const LiveActor*));
const sead::Vector3f& getFarPlayerPosMaxX(const LiveActor*);
const sead::Vector3f& getFarPlayerPosMinX(const LiveActor*);
u32 calcPlayerListOrderByDistance(const LiveActor*, const LiveActor**, u32);
u32 calcAlivePlayerActor(const LiveActor*, const LiveActor**, u32);
LiveActor* tryFindNearestPlayerActorCondition(const LiveActor*, bool (*)(const LiveActor*));
bool tryFindNearestPlayerPosCondition(sead::Vector3f*, const LiveActor*,
                                      bool (*)(const LiveActor*));
bool isResetablePlayerPos(const LiveActor*, const sead::Vector3f&, f32, f32);
bool isResetablePlayerPos(const LiveActor*, f32);
void faceToPlayer(LiveActor*);
}  // namespace al

namespace alPlayerFunction {
void registerPlayer(al::LiveActor*, al::PadRumbleKeeper*);
bool isFullPlayerHolder(al::LiveActor*);
s32 findPlayerHolderIndex(const al::LiveActor*);
s32 findPlayerHolderIndex(const al::HitSensor*);
bool isPlayerActor(const al::LiveActor*);
bool isPlayerActor(const al::HitSensor*);
}  // namespace alPlayerFunction
