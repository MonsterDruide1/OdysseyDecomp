#include "Library/Player/PlayerUtil.h"

#include "Library/Controller/PadRumbleDirector.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSceneInfo.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Player/PlayerHolder.h"
#include "Project/Controller/PadRumbleKeeper.h"

namespace al {

s32 getPlayerNumMax(const LiveActor* actor) {
    return getPlayerNumMax(actor->getSceneInfo()->playerHolder);
}

s32 getPlayerNumMax(const PlayerHolder* holder) {
    return holder->getPlayerNum();
}

s32 getAlivePlayerNum(const LiveActor* actor) {
    return getAlivePlayerNum(actor->getSceneInfo()->playerHolder);
}

s32 getAlivePlayerNum(const PlayerHolder* holder) {
    s32 playerNum = getPlayerNumMax(holder);
    s32 alivePlayers = 0;

    for (s32 i = 0; i < playerNum; i++) {
        LiveActor* player = getPlayerActor(holder, i);
        if (isAlive(player))
            alivePlayers++;
    }

    return alivePlayers;
}

LiveActor* getPlayerActor(const LiveActor* actor, s32 index) {
    return getPlayerActor(actor->getSceneInfo()->playerHolder, index);
}

LiveActor* getPlayerActor(const PlayerHolder* holder, s32 index) {
    return tryGetPlayerActor(holder, index);
}

const sead::Vector3f& getPlayerPos(const LiveActor* actor, s32 index) {
    return getPlayerPos(actor->getSceneInfo()->playerHolder, index);
}

const sead::Vector3f& getPlayerPos(const PlayerHolder* holder, s32 index) {
    return getTrans(getPlayerActor(holder, index));
}

LiveActor* tryGetPlayerActor(const LiveActor* actor, s32 index) {
    return tryGetPlayerActor(actor->getSceneInfo()->playerHolder, index);
}

LiveActor* tryGetPlayerActor(const PlayerHolder* holder, s32 index) {
    return holder->tryGetPlayer(index);
}

bool isPlayerDead(const LiveActor* actor, s32 index) {
    return isPlayerDead(actor->getSceneInfo()->playerHolder, index);
}

bool isPlayerDead(const PlayerHolder* holder, s32 index) {
    return isDead(getPlayerActor(holder, index));
}

bool isPlayerAreaTarget(const LiveActor* actor, s32 index) {
    return isPlayerAreaTarget(actor->getSceneInfo()->playerHolder, index);
}

bool isPlayerAreaTarget(const PlayerHolder* holder, s32 index) {
    return isAreaTarget(getPlayerActor(holder, index));
}

LiveActor* tryFindAlivePlayerActorFirst(const LiveActor* actor) {
    return tryFindAlivePlayerActorFirst(actor->getSceneInfo()->playerHolder);
}

LiveActor* tryFindAlivePlayerActorFirst(const PlayerHolder* holder) {
    u32 playerNum = getPlayerNumMax(holder);

    for (u32 i = 0; i < playerNum; i++) {
        LiveActor* player = holder->tryGetPlayer(i);
        if (!isDead(player))
            return player;
    }

    return nullptr;
}

LiveActor* findAlivePlayerActorFirst(const LiveActor* actor) {
    return findAlivePlayerActorFirst(actor->getSceneInfo()->playerHolder);
}

LiveActor* findAlivePlayerActorFirst(const PlayerHolder* holder) {
    return tryFindAlivePlayerActorFirst(holder);
}

static inline PadRumbleKeeper* getPlayerPadRumbleKeeper(const PlayerHolder* holder, s32 index) {
    return holder->getPadRumbleKeeper(index);
}

PadRumbleKeeper* getPlayerPadRumbleKeeper(const LiveActor* actor, s32 index) {
    return getPlayerPadRumbleKeeper(actor->getSceneInfo()->playerHolder, index);
}

s32 getPlayerPort(const PlayerHolder* holder, s32 index) {
    return getPlayerPadRumbleKeeper(holder, index)->getPort();
}

s32 getPlayerPort(const LiveActor* actor, s32 index) {
    return getPlayerPort(actor->getSceneInfo()->playerHolder, index);
}

LiveActor* findAlivePlayerActorFromPort(const PlayerHolder* holder, s32 port) {
    return tryFindAlivePlayerActorFromPort(holder, port);
}

LiveActor* tryFindAlivePlayerActorFromPort(const PlayerHolder* holder, s32 port) {
    s32 playerNum = getPlayerNumMax(holder);

    for (s32 i = 0; i < playerNum; i++) {
        LiveActor* player = tryGetPlayerActor(holder, i);
        if (getPlayerPort(player, i) == port && !isPlayerDead(player, i))
            return player;
    }

    return nullptr;
}

LiveActor* findAlivePlayerActorFromPort(const LiveActor* actor, s32 port) {
    return tryFindAlivePlayerActorFromPort(actor, port);
}

LiveActor* tryFindAlivePlayerActorFromPort(const LiveActor* actor, s32 port) {
    return tryFindAlivePlayerActorFromPort(actor->getSceneInfo()->playerHolder, port);
}

s32 findNearestPlayerIdCondition(const LiveActor* actor, const sead::Vector3f& pos,
                                 bool (*condition)(const LiveActor*), f32 threshold) {
    PlayerHolder* holder = actor->getSceneInfo()->playerHolder;
    s32 playerNum = getPlayerNumMax(holder);

    f32 minDistance = sead::Mathf::maxNumber();
    s32 nearestPlayerId = -1;
    for (s32 i = 0; i < playerNum; i++) {
        LiveActor* player = holder->getPlayer(i);
        if (!player || condition(player))
            continue;
        const sead::Vector3f& playerPos = getTrans(player);
        f32 distance = (playerPos - pos).squaredLength();
        if (distance < minDistance) {
            minDistance = distance;
            nearestPlayerId = i;
        }
    }

    if (threshold * threshold < minDistance && threshold > 0.0f)
        return -1;

    return nearestPlayerId;
}

s32 findNearestPlayerId(const LiveActor* actor, f32 threshold) {
    return findNearestPlayerIdCondition(actor, getTrans(actor), &isDead, threshold);
}

LiveActor* findNearestPlayerActor(const LiveActor* actor) {
    return getPlayerActor(actor, findNearestPlayerId(actor, -1.0f));
}

LiveActor* tryFindNearestPlayerActor(const LiveActor* actor) {
    s32 nearestPlayerId = findNearestPlayerId(actor, -1.0f);
    if (nearestPlayerId < 0)
        return nullptr;
    return getPlayerActor(actor, nearestPlayerId);
}

const sead::Vector3f& findNearestPlayerPos(const LiveActor* actor) {
    return getPlayerPos(actor, findNearestPlayerId(actor, -1.0f));
}

bool tryFindNearestPlayerPos(sead::Vector3f* pos, const LiveActor* actor) {
    s32 nearestPlayerId = findNearestPlayerId(actor, -1.0f);
    if (nearestPlayerId < 0)
        return false;
    LiveActor* player = getPlayerActor(actor, nearestPlayerId);
    if (!player)
        return false;

    *pos = getTrans(player);
    return true;
}

// NON_MATCHING: regalloc (https://decomp.me/scratch/XLeWX)
bool tryFindNearestPlayerDisatanceFromTarget(f32* distance, const LiveActor* actor,
                                             const sead::Vector3f& target) {
    s32 nearestPlayerId = findNearestPlayerIdCondition(actor, target, &isDead, -1.0f);
    if (nearestPlayerId < 0)
        return false;

    LiveActor* player = getPlayerActor(actor, nearestPlayerId);
    if (!player)
        return false;

    *distance = (getTrans(player) - target).length();
    return true;
}

bool isNearPlayer(const LiveActor* actor, f32 threshold) {
    return findNearestPlayerId(actor, threshold) >= 0;
}

// BUG: This only checks the nearest player (overall), not all players
bool isNearPlayerH(const LiveActor* actor, f32 threshold) {
    s32 nearestPlayerId = findNearestPlayerId(actor, -1.0f);
    if (nearestPlayerId < 0)
        return false;

    sead::Vector3f playerPos = getPlayerPos(actor, nearestPlayerId);
    sead::Vector3f diff = playerPos - getTrans(actor);
    f32 hDiff = diff.dot(getGravity(actor));
    return (diff - getGravity(actor) * hDiff).length() <= threshold;
}

// BUG: This only checks the nearest player (overall), not all players
bool isNearPlayerHCondition(const LiveActor* actor, f32 threshold,
                            bool (*condition)(const LiveActor*)) {
    s32 nearestPlayerId = findNearestPlayerIdCondition(actor, getTrans(actor), condition, -1.0f);
    if (nearestPlayerId < 0)
        return false;

    return calcDistanceH(getPlayerActor(actor, nearestPlayerId), actor) <= threshold;
}

const sead::Vector3f& getFarPlayerPosMaxX(const LiveActor* actor) {
    PlayerHolder* holder = actor->getSceneInfo()->playerHolder;
    getTrans(actor);

    const LiveActor* farPlayer = nullptr;
    f32 maxX = -1.0f;
    for (s32 i = 0; i < getPlayerNumMax(holder); i++) {
        LiveActor* player = holder->getPlayer(i);
        if (!farPlayer || maxX < getTrans(player).x) {
            maxX = getTrans(player).x;
            farPlayer = player;
        }
    }

    return getTrans(farPlayer);
}

const sead::Vector3f& getFarPlayerPosMinX(const LiveActor* actor) {
    PlayerHolder* holder = actor->getSceneInfo()->playerHolder;
    getTrans(actor);

    const LiveActor* farPlayer = nullptr;
    f32 minX = -1.0f;
    for (s32 i = 0; i < getPlayerNumMax(holder); i++) {
        LiveActor* player = holder->getPlayer(i);
        if (!farPlayer || getTrans(player).x < minX) {
            minX = getTrans(player).x;
            farPlayer = player;
        }
    }

    return getTrans(farPlayer);
}

u32 calcPlayerListOrderByDistance(const LiveActor* actor, const LiveActor** actorList, u32 size) {
    u32 playerNum = getPlayerNumMax(actor);
    const sead::Vector3f& pos = getTrans(actor);

    f32 distances[64];
    for (s32 i = 0; i != playerNum; i++) {
        LiveActor* player = getPlayerActor(actor, i);
        f32 distance = sead::Mathf::maxNumber();
        if (!isDead(player)) {
            sead::Vector3f playerPos = getTrans(player);
            distance = (playerPos - pos).squaredLength();
        }
        distances[i] = distance;
    }

    for (s32 i = 0; i < size; i++) {
        f32 min_distance = sead::Mathf::maxNumber();
        s32 min_index = -1;

        for (u32 i = 0; i < playerNum; i++) {
            if (distances[i] <= min_distance) {
                min_distance = distances[i];
                min_index = i;
            }
        }

        if (min_index == -1)
            return i;

        actorList[i] = getPlayerActor(actor, min_index);
        distances[min_index] = sead::Mathf::maxNumber();
    }

    return size;
}

// NON_MATCHING: major mismatch, it's doing something weird with flags
// (https://decomp.me/scratch/c1lVL)
u32 calcAlivePlayerActor(const LiveActor* actor, const LiveActor** actorList, u32 size) {
    u32 playerNum = getPlayerNumMax(actor);
    u32 resultNum = 0;
    for (u32 i = 0; i != playerNum; i++) {
        LiveActor* player = getPlayerActor(actor, i);
        if (!isDead(player)) {
            actorList[resultNum] = player;
            resultNum++;
            if (resultNum >= size)
                return size;
        }
    }
    return resultNum;
}

LiveActor* tryFindNearestPlayerActorCondition(const LiveActor* actor,
                                              bool (*condition)(const LiveActor*)) {
    s32 nearestPlayerId = findNearestPlayerIdCondition(actor, getTrans(actor), condition, -1.0f);
    if (nearestPlayerId < 0)
        return nullptr;
    return getPlayerActor(actor, nearestPlayerId);
}

bool tryFindNearestPlayerPosCondition(sead::Vector3f* pos, const LiveActor* actor,
                                      bool (*condition)(const LiveActor*)) {
    LiveActor* nearestPlayer = tryFindNearestPlayerActorCondition(actor, condition);
    if (!nearestPlayer)
        return false;
    *pos = getTrans(nearestPlayer);
    return true;
}

bool isResetablePlayerPos(const LiveActor* actor, const sead::Vector3f& pos, f32 clippingRadius,
                          f32 threshold) {
    if (isJudgedToClipFrustum(actor, pos, clippingRadius, 300.0f) &&
        !isNearPlayer(actor, threshold))
        return true;
    return false;
}

bool isResetablePlayerPos(const LiveActor* actor, f32 threshold) {
    return isResetablePlayerPos(actor, getTrans(actor), getClippingRadius(actor), threshold);
}

void faceToPlayer(LiveActor* actor) {
    sead::Vector3f dir;
    calcDirToActorH(&dir, actor, getPlayerActor(actor, 0));
    faceToDirection(actor, dir);
}

}  // namespace al

namespace alPlayerFunction {

void registerPlayer(al::LiveActor* actor, al::PadRumbleKeeper* padRumbleKeeper) {
    return actor->getSceneInfo()->playerHolder->registerPlayer(actor, padRumbleKeeper);
}

bool isFullPlayerHolder(al::LiveActor* actor) {
    return actor->getSceneInfo()->playerHolder->isFull();
}

s32 findPlayerHolderIndex(const al::LiveActor* actor) {
    al::PlayerHolder* playerHolder = actor->getSceneInfo()->playerHolder;
    s32 playerNum = playerHolder->getPlayerNum();
    for (s32 i = 0; i < playerNum; i++)
        if (playerHolder->getPlayer(i) == actor)
            return i;
    return 0;
}

s32 findPlayerHolderIndex(const al::HitSensor* sensor) {
    return findPlayerHolderIndex(al::getSensorHost(sensor));
}

bool isPlayerActor(const al::LiveActor* actor) {
    al::PlayerHolder* playerHolder = actor->getSceneInfo()->playerHolder;
    s32 playerNum = playerHolder->getPlayerNum();
    for (s32 i = 0; i < playerNum; i++)
        if (playerHolder->getPlayer(i) == actor)
            return true;
    return false;
}

bool isPlayerActor(const al::HitSensor* sensor) {
    return isPlayerActor(al::getSensorHost(sensor));
}

}  // namespace alPlayerFunction
