#pragma once

#include <math/seadVector.h>

namespace al {
class HitSensor;
class LiveActor;
class PadRumbleKeeper;
class PlayerHolder;

s32 getPlayerNumMax(const LiveActor*);
s32 getPlayerNumMax(const PlayerHolder*);
s32 getAlivePlayerNum(const LiveActor*);
s32 getAlivePlayerNum(const PlayerHolder*);
LiveActor* getPlayerActor(const LiveActor*, s32);
LiveActor* getPlayerActor(const PlayerHolder*, s32);
const sead::Vector3f& getPlayerPos(const LiveActor*, s32);
const sead::Vector3f& getPlayerPos(const PlayerHolder*, s32);
LiveActor* tryGetPlayerActor(const LiveActor*, s32);
LiveActor* tryGetPlayerActor(const PlayerHolder*, s32);
bool isPlayerDead(const LiveActor*, s32);
bool isPlayerDead(const PlayerHolder*, s32);
bool isPlayerAreaTarget(const LiveActor*, s32);
bool isPlayerAreaTarget(const PlayerHolder*, s32);
LiveActor* tryFindAlivePlayerActorFirst(const LiveActor*);
LiveActor* tryFindAlivePlayerActorFirst(const PlayerHolder*);
LiveActor* findAlivePlayerActorFirst(const LiveActor*);
LiveActor* findAlivePlayerActorFirst(const PlayerHolder*);
al::PadRumbleKeeper* getPlayerPadRumbleKeeper(const LiveActor*, s32);
s32 getPlayerPort(const PlayerHolder*, s32);
s32 getPlayerPort(const LiveActor*, s32);
LiveActor* findAlivePlayerActorFromPort(const PlayerHolder*, s32);
LiveActor* tryFindAlivePlayerActorFromPort(const PlayerHolder*, s32);
LiveActor* findAlivePlayerActorFromPort(const LiveActor*, s32);
LiveActor* tryFindAlivePlayerActorFromPort(const LiveActor*, s32);
s32 findNearestPlayerId(const LiveActor*, f32);
LiveActor* findNearestPlayerActor(const LiveActor*);
LiveActor* tryFindNearestPlayerActor(const LiveActor*);
void findNearestPlayerPos(const LiveActor*);
void tryFindNearestPlayerPos(sead::Vector3f*, const LiveActor*);
void tryFindNearestPlayerDisatanceFromTarget(f32*, const LiveActor*, const sead::Vector3f&);
bool isNearPlayer(const LiveActor*, f32);
bool isNearPlayerH(const LiveActor*, f32);
bool isNearPlayerHCondition(const LiveActor*, f32, bool (*)(const LiveActor*));
void getFarPlayerPosMaxX(const LiveActor*);
void getFarPlayerPosMinX(const LiveActor*);
void calcPlayerListOrderByDistance(const LiveActor*, const LiveActor**, u32);
void calcAlivePlayerActor(const LiveActor*, const LiveActor**, u32);
void tryFindNearestPlayerActorCondition(const LiveActor*, bool (*)(const LiveActor*));
void tryFindNearestPlayerPosCondition(sead::Vector3f*, const LiveActor*,
                                      bool (*)(const LiveActor*));
bool isResetablePlayerPos(const LiveActor*, const sead::Vector3f&, f32, f32);
bool isResetablePlayerPos(const LiveActor*, f32);
void faceToPlayer(LiveActor*);
}  // namespace al

namespace alPlayerFunction {
void registerPlayer(al::LiveActor*, al::PadRumbleKeeper*);
bool isFullPlayerHolder(al::LiveActor*);
void findPlayerHolderIndex(const al::LiveActor*);
void findPlayerHolderIndex(al::HitSensor const*);
bool isPlayerActor(const al::LiveActor*);
bool isPlayerActor(al::HitSensor const*);
}  // namespace alPlayerFunction
