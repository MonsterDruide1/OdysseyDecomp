#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
class PlayerHolder;

s32 getPlayerNumMax(const PlayerHolder* holder);
s32 getAlivePlayerNum(const PlayerHolder* holder);
LiveActor* getPlayerActor(const PlayerHolder* holder, s32 index);
const sead::Vector3f& getPlayerPos(const PlayerHolder* holder, s32 index);
LiveActor* tryGetPlayerActor(const PlayerHolder* holder, s32 index);
bool isPlayerDead(const PlayerHolder* holder, s32 index);
bool isPlayerAreaTarget(const PlayerHolder* holder, s32 index);
LiveActor* tryFindAlivePlayerActorFirst(const PlayerHolder* holder);
LiveActor* findAlivePlayerActorFirst(const PlayerHolder* holder);
}  // namespace al
