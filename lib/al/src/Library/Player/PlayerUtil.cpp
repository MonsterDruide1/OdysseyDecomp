#include "Library/Player/PlayerUtil.h"

#include "Library/Player/PlayerHolder.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/LiveActor/ActorPoseKeeper.h"

namespace al {

int getPlayerNumMax(const PlayerHolder* holder) {
    return holder->getPlayerNum();
}

int getAlivePlayerNum(const PlayerHolder* holder) {
    int player_num = holder->getPlayerNum();
    int alive_players = 0;

    for (int i = 0; i < player_num; i++) {
        LiveActor* player = holder->tryGetPlayer(i);
        if (isAlive(player)) {
            alive_players++;
        }
    }

    return alive_players;
}

LiveActor* getPlayerActor(const PlayerHolder* holder, int index) {
    return holder->tryGetPlayer(index);
}

const sead::Vector3f& getPlayerPos(const PlayerHolder* holder, int index) {
    LiveActor* player = holder->tryGetPlayer(index);
    return getTrans(player);
}

LiveActor* tryGetPlayerActor(const PlayerHolder* holder, int index) {
    return holder->tryGetPlayer(index);
}

bool isPlayerDead(const PlayerHolder* holder, int index) {
    LiveActor* player = holder->tryGetPlayer(index);
    return isDead(player);
}

bool isPlayerAreaTarget(const PlayerHolder* holder, int index) {
    LiveActor* player = holder->tryGetPlayer(index);
    return isAreaTarget(player);
}

LiveActor* tryFindAlivePlayerActorFirst(const PlayerHolder* holder) {
    u32 player_num = holder->getPlayerNum();

    for (u32 i = 0; i < player_num; i++) {
        LiveActor* player = holder->tryGetPlayer(i);
        if (!isDead(player)) {
            return player;
        }
    }

    return nullptr;
}

LiveActor* findAlivePlayerActorFirst(const PlayerHolder* holder) {
    u32 player_num = holder->getPlayerNum();

    for (u32 i = 0; i < player_num; i++) {
        LiveActor* player = holder->tryGetPlayer(i);
        if (!isDead(player)) {
            return player;
        }
    }

    return nullptr;
}
}  // namespace al
