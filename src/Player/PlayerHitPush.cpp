#include "Player/PlayerHitPush.h"

#include "Library/LiveActor/ActorPoseUtil.h"

#include "Player/PlayerConst.h"

PlayerHitPush::PlayerHitPush(const al::LiveActor* player, const PlayerConst* playerConst)
    : mPlayer(player), mPlayerConst(playerConst) {}

void PlayerHitPush::clearHitFlag() {
    mIsHit = false;
    mIsBlowDown = false;
    mPush = sead::Vector3f::zero;
}

void PlayerHitPush::setHitPush(const sead::Vector3f& dir) {
    setHitInfo(dir, mPlayerConst->getPushPowerDamage(), mPlayerConst->getHopPowerDamage());
}

void PlayerHitPush::setHitInfo(const sead::Vector3f& dir, f32 pushPower, f32 hopPower) {
    mIsHit = true;
    mPush.setAdd(dir * -pushPower, al::getGravity(mPlayer) * -hopPower);
}

void PlayerHitPush::setHitPushStrong(const sead::Vector3f& dir) {
    setHitInfo(dir, 5.0f, 20.0f);
}

void PlayerHitPush::setHitPushVeryStrong(const sead::Vector3f& dir) {
    setHitInfo(dir, 15.0f, 10.0f);
}

void PlayerHitPush::setHitPushBlowDown(const sead::Vector3f& dir) {
    setHitPushVeryStrong(dir);
    mIsBlowDown = true;
}
