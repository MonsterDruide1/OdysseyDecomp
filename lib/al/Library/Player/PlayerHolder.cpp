#include "Library/Player/PlayerHolder.h"

namespace al {

PlayerHolder::PlayerHolder(s32 count) {
    mBufferSize = count;
    mPlayers = new Player[count];
    clear();
}

void PlayerHolder::clear() {
    for (s32 i = 0; i < mBufferSize; i++) {
        mPlayers[i].actor = nullptr;
        mPlayers[i].padRumbleKeeper = nullptr;
    }
}

void PlayerHolder::registerPlayer(LiveActor* actor, PadRumbleKeeper* rumbleKeeper) {
    mPlayers[mPlayerNum].actor = actor;
    mPlayers[mPlayerNum].padRumbleKeeper = rumbleKeeper;
    mPlayerNum++;
}

LiveActor* PlayerHolder::getPlayer(s32 index) const {
    return mPlayers[index].actor;
}

LiveActor* PlayerHolder::tryGetPlayer(s32 index) const {
    if (mBufferSize <= index)
        return nullptr;
    else if (mPlayerNum <= index)
        return nullptr;

    return mPlayers[index].actor;
}

s32 PlayerHolder::getPlayerNum() const {
    return mPlayerNum;
}

s32 PlayerHolder::getBufferSize() const {
    return mBufferSize;
}

bool PlayerHolder::isFull() const {
    return mBufferSize <= mPlayerNum;
}

bool PlayerHolder::isExistPadRumbleKeeper(s32 index) const {
    return mPlayers[index].padRumbleKeeper != nullptr;
}

PadRumbleKeeper* PlayerHolder::getPadRumbleKeeper(s32 index) const {
    return mPlayers[index].padRumbleKeeper;
}
}  // namespace al
