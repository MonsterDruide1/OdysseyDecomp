#include "al/player/PlayerHolder.h"

namespace al {

PlayerHolder::PlayerHolder(int count) {
    mPlayers = nullptr;
    mPlayerNum = 0;
    mBufferSize = count;

    mPlayers = new Player[count];
    clear();
}

void PlayerHolder::clear() {
    for (int i = 0; i < mBufferSize; i++) {
        mPlayers[i].mActor = nullptr;
        mPlayers[i].mPadRumbleKeeper = nullptr;
    }
}

void PlayerHolder::registerPlayer(LiveActor* actor, PadRumbleKeeper* rumble_keeper) {
    mPlayers[mPlayerNum].mActor = actor;
    mPlayers[mPlayerNum].mPadRumbleKeeper = rumble_keeper;
    mPlayerNum++;
}

LiveActor* PlayerHolder::getPlayer(int index) const {
    return mPlayers[index].mActor;
}

LiveActor* PlayerHolder::tryGetPlayer(int index) const {
    if (mBufferSize <= index) {
        return nullptr;
    } else if (mPlayerNum <= index) {
        return nullptr;
    }

    return mPlayers[index].mActor;
}

int PlayerHolder::getPlayerNum() const {
    return mPlayerNum;
}

int PlayerHolder::getBufferSize() const {
    return mBufferSize;
}

bool PlayerHolder::isFull() const {
    return mBufferSize <= mPlayerNum;
}

bool PlayerHolder::isExistPadRumbleKeeper(int index) const {
    return mPlayers[index].mPadRumbleKeeper != nullptr;
}

PadRumbleKeeper* PlayerHolder::getPadRumbleKeeper(int index) const {
    return mPlayers[index].mPadRumbleKeeper;
}
}  // namespace al
