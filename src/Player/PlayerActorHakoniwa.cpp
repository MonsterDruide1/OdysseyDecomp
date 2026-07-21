#include "Player/PlayerActorHakoniwa.h"

PlayerPuppet* PlayerActorHakoniwa::getPlayerPuppet() {
    return mPuppet;
}

PlayerInfo* PlayerActorHakoniwa::getPlayerInfo() const {
    return mInfo;
}

PlayerHackKeeper* PlayerActorHakoniwa::getPlayerHackKeeper() const {
    return mHackKeeper;
}

bool PlayerActorHakoniwa::tryActionCapSpinAttack() {
    return tryActionCapSpinAttackImpl(true);
}

bool PlayerActorHakoniwa::tryActionCapSpinAttackMiss() {
    return tryActionCapSpinAttackImpl(false);
}

void PlayerActorHakoniwa::exeDemo() {}

void PlayerActorHakoniwa::exeDead() {}
