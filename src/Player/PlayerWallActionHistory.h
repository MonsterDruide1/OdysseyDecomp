#pragma once

#include <math/seadVector.h>

class IUsePlayerCollision;

class PlayerWallActionHistory {
public:
    PlayerWallActionHistory();
    void update(const IUsePlayerCollision* collider);
    void reset();
    void recordWallJump(const IUsePlayerCollision* collider, const sead::Vector3f& position);
    void recordWallJump(const sead::Vector3f& position, const sead::Vector3f& normal);
    void recordWallLeave(const sead::Vector3f& position, const sead::Vector3f& normal);

    bool isJumpStored() const { return mIsJumpStored; }
    const sead::Vector3f& getJumpWallPosition() const { return mJumpWallPosition; }
    const sead::Vector3f& getJumpWallNormal() const { return mJumpWallNormal; }
    bool isLeaveStored() const { return mIsLeaveStored; }
    const sead::Vector3f& getLeaveWallPosition() const { return mLeaveWallPosition; }

private:
    bool mIsJumpStored = false;
    sead::Vector3f mJumpWallPosition = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mJumpWallNormal = {0.0f, 0.0f, 0.0f};
    bool mIsLeaveStored = false;
    sead::Vector3f mLeaveWallPosition = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mLeaveWallNormal = {0.0f, 0.0f, 0.0f};
};
static_assert(sizeof(PlayerWallActionHistory) == 0x38);
