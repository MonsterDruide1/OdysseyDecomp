#include "Library/Player/PlayerHolder.h"

namespace al {

PlayerHolder::PlayerHolder(s32 count) {
    mBufferSize = count;
    mPlayers = new Player[count];
    clear();
}

void PlayerHolder::clear() {
    for (s32 i = 0; i < mBufferSize; i++) {
        mPlayers[i].mActor = nullptr;
        mPlayers[i].mPadRumbleKeeper = nullptr;
    }
}

void PlayerHolder::registerPlayer(LiveActor* actor, PadRumbleKeeper* rumbleKeeper) {
    mPlayers[mPlayerNum].mActor = actor;
    mPlayers[mPlayerNum].mPadRumbleKeeper = rumbleKeeper;
    mPlayerNum++;
}

LiveActor* PlayerHolder::getPlayer(s32 index) const {
    return mPlayers[index].mActor;
}

LiveActor* PlayerHolder::tryGetPlayer(s32 index) const {
    if (mBufferSize <= index)
        return nullptr;
    else if (mPlayerNum <= index)
        return nullptr;

    return mPlayers[index].mActor;
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
    return mPlayers[index].mPadRumbleKeeper != nullptr;
}

PadRumbleKeeper* PlayerHolder::getPadRumbleKeeper(s32 index) const {
    return mPlayers[index].mPadRumbleKeeper;
}
}  // namespace al
