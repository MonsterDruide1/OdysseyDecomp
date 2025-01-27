#include "Player/PlayerAnimFrameCtrl.h"

#include "Library/LiveActor/ActorActionFunction.h"

PlayerAnimFrameCtrl::PlayerAnimFrameCtrl() = default;

void PlayerAnimFrameCtrl::startAction(al::LiveActor* player, const sead::SafeString& name) {
    mActionName = name;
    mIsActionRepeat = !al::isActionOneTime(player, name.cstr());
    mCurrentFrame = 0.0f;
    mActionFrameMax = al::getActionFrameMax(player, name.cstr());
    mRate = 1.0f;
    mIsSkipUpdate = true;
}

void PlayerAnimFrameCtrl::changeActionName(al::LiveActor* player, const sead::SafeString& name) {
    mActionName = name;
    mActionFrameMax = al::getActionFrameMax(player, name.cstr());
}

void PlayerAnimFrameCtrl::update() {
    if (mIsSkipUpdate) {
        mIsSkipUpdate = false;
        return;
    }

    mCurrentFrame += mRate;
    if (mCurrentFrame >= mActionFrameMax) {
        if (mIsActionRepeat)
            mCurrentFrame -= mActionFrameMax;
        else
            mCurrentFrame = mActionFrameMax;
    }
}

void PlayerAnimFrameCtrl::updateSync(al::LiveActor* player) {
    mIsSkipUpdate = false;
    mRate = al::getActionFrameRate(player);
    mCurrentFrame = al::getActionFrame(player) + mRate;
}

void PlayerAnimFrameCtrl::setFrame(f32 frame) {
    if (mIsActionRepeat) {
        // doing modf manually
        mCurrentFrame = frame - (mActionFrameMax * (u32)(frame / mActionFrameMax));
    } else if (mCurrentFrame > mActionFrameMax) {
        mCurrentFrame = mActionFrameMax;
    }

    mIsSkipUpdate = true;
}

const char* PlayerAnimFrameCtrl::getActionName() const {
    return mActionName.cstr();
}

f32 PlayerAnimFrameCtrl::getCurrentFrame() const {
    return mCurrentFrame;
}

f32 PlayerAnimFrameCtrl::getRate() const {
    return mRate;
}

bool PlayerAnimFrameCtrl::isActionEnd() const {
    return !mIsActionRepeat && mCurrentFrame >= mActionFrameMax;
}
