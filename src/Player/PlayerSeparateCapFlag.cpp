#include "Player/PlayerSeparateCapFlag.h"

PlayerSeparateCapFlag::PlayerSeparateCapFlag(al::LiveActor* player)
    : mPlayer(player), mIsSeparateMode(false), mIsHipDrop(false), mIsHipDropCancel(false),
      mIsCapTouchJump(false) {}

bool PlayerSeparateCapFlag::isEnableHipDrop() const {
    return mIsHipDrop;
}

bool PlayerSeparateCapFlag::isEnableHipDropCancel() const {
    return mIsHipDropCancel;
}

bool PlayerSeparateCapFlag::isEnableCapTouchJump() const {
    return mIsCapTouchJump;
}

bool PlayerSeparateCapFlag::isEnableGroundPound() const {
    return false;
}

bool PlayerSeparateCapFlag::isSeparateMode() const {
    return mIsSeparateMode;
}

void PlayerSeparateCapFlag::setSeparateMode(bool val) {
    mIsSeparateMode = val;
}

void PlayerSeparateCapFlag::setHipDrop(bool val) {
    mIsHipDrop = val;
}

void PlayerSeparateCapFlag::setCapTouchJump(bool val) {
    mIsCapTouchJump = val;
}
