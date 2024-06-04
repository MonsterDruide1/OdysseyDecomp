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
LiveActor* getPlayerActor(const LiveActor*, int);
LiveActor* getPlayerActor(const PlayerHolder*, int);
const sead::Vector3f& getPlayerPos(const LiveActor*, int);
const sead::Vector3f& getPlayerPos(const PlayerHolder*, int);
LiveActor* tryGetPlayerActor(const LiveActor*, int);
LiveActor* tryGetPlayerActor(const PlayerHolder*, int);
bool isPlayerDead(const LiveActor*, int);
bool isPlayerDead(const PlayerHolder*, int);
bool isPlayerAreaTarget(const LiveActor*, int);
bool isPlayerAreaTarget(const PlayerHolder*, int);
LiveActor* tryFindAlivePlayerActorFirst(const LiveActor*);
LiveActor* tryFindAlivePlayerActorFirst(const PlayerHolder*);
LiveActor* findAlivePlayerActorFirst(const LiveActor*);
LiveActor* findAlivePlayerActorFirst(const PlayerHolder*);
al::PadRumbleKeeper* getPlayerPadRumbleKeeper(const LiveActor*, int);
s32 getPlayerPort(const PlayerHolder*, int);
s32 getPlayerPort(const LiveActor*, int);
LiveActor* findAlivePlayerActorFromPort(const PlayerHolder*, int);
LiveActor* tryFindAlivePlayerActorFromPort(const PlayerHolder*, int);
LiveActor* findAlivePlayerActorFromPort(const LiveActor*, int);
LiveActor* tryFindAlivePlayerActorFromPort(const LiveActor*, int);
s32 findNearestPlayerId(const LiveActor*, float);
LiveActor* findNearestPlayerActor(const LiveActor*);
LiveActor* tryFindNearestPlayerActor(const LiveActor*);
void findNearestPlayerPos(const LiveActor*);
void tryFindNearestPlayerPos(sead::Vector3f*, const LiveActor*);
void tryFindNearestPlayerDisatanceFromTarget(float*, const LiveActor*, const sead::Vector3f&);
bool isNearPlayer(const LiveActor*, float);
bool isNearPlayerH(const LiveActor*, float);
bool isNearPlayerHCondition(const LiveActor*, float, bool (*)(const LiveActor*));
void getFarPlayerPosMaxX(const LiveActor*);
void getFarPlayerPosMinX(const LiveActor*);
void calcPlayerListOrderByDistance(const LiveActor*, const LiveActor**, u32);
void calcAlivePlayerActor(const LiveActor*, const LiveActor**, u32);
void tryFindNearestPlayerActorCondition(const LiveActor*, bool (*)(const LiveActor*));
void tryFindNearestPlayerPosCondition(sead::Vector3f*, const LiveActor*,
                                      bool (*)(const LiveActor*));
bool isResetablePlayerPos(const LiveActor*, const sead::Vector3f&, float, float);
bool isResetablePlayerPos(const LiveActor*, float);
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
