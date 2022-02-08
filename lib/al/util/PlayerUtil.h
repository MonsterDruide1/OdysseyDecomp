#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
class PlayerHolder;

int getPlayerNumMax(const PlayerHolder* holder);
int getAlivePlayerNum(const PlayerHolder* holder);
LiveActor* getPlayerActor(const PlayerHolder* holder, int index);
const sead::Vector3f& getPlayerPos(const PlayerHolder* holder, int index);
LiveActor* tryGetPlayerActor(const PlayerHolder* holder, int index);
bool isPlayerDead(const PlayerHolder* holder, int index);
bool isPlayerAreaTarget(const PlayerHolder* holder, int index);
LiveActor* tryFindAlivePlayerActorFirst(const PlayerHolder* holder);
LiveActor* findAlivePlayerActorFirst(const PlayerHolder* holder);
}  // namespace al
