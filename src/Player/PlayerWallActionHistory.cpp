#include "Player/PlayerWallActionHistory.h"

#include "Util/PlayerCollisionUtil.h"

PlayerWallActionHistory::PlayerWallActionHistory() = default;

void PlayerWallActionHistory::update(const IUsePlayerCollision* collider) {
    if (rs::isCollidedGround(collider))
        reset();
}

void PlayerWallActionHistory::reset() {
    mIsJumpStored = false;
    mIsLeaveStored = false;
}

void PlayerWallActionHistory::recordWallJump(const IUsePlayerCollision* collider,
                                             const sead::Vector3f& position) {
    mIsJumpStored = true;
    mJumpWallPosition = position;
    mJumpWallNormal = rs::getCollidedWallNormal(collider);
}

void PlayerWallActionHistory::recordWallJump(const sead::Vector3f& position,
                                             const sead::Vector3f& normal) {
    mIsJumpStored = true;
    mJumpWallPosition = position;
    mJumpWallNormal = normal;
}

void PlayerWallActionHistory::recordWallLeave(const sead::Vector3f& position,
                                              const sead::Vector3f& normal) {
    mIsLeaveStored = true;
    mLeaveWallPosition = position;
    mLeaveWallNormal = normal;
}
