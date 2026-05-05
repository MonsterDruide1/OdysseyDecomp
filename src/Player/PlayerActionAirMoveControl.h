#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
}
class IUsePlayerHack;
class PlayerConst;
class PlayerInput;
class IUsePlayerCollision;
class PlayerActionTurnControl;

class PlayerActionAirMoveControl {
public:
    PlayerActionAirMoveControl(al::LiveActor* player, const PlayerConst* pConst,
                               const PlayerInput* input, const IUsePlayerCollision* collision,
                               bool isSlerpGravity);
    void setup(f32 speedMax, f32 velocityH, s32 extendFrame, f32 velocityV, f32 gravity,
               s32 noInputFrame, f32 jumpInertia);
    void setupTurn(f32 turnAngleStart, f32 turnAngleLimit, f32 turnAngleFast,
                   f32 turnAngleFastLimit, s32 turnAccelFrame, s32 turnAccelFrameFast,
                   s32 turnBrakeFrame);
    void setExtendFrame(s32 extendFrame);
    void setupCollideWallScaleVelocity(f32 scaleH, f32 scaleV, f32 scaleLimit);
    void verticalizeStartMoveDir(const sead::Vector3f& up);
    void update();
    bool isHoldJumpExtend() const;
    void calcMoveInput(sead::Vector3f* moveInput, const sead::Vector3f& up) const;

    void setHacker(IUsePlayerHack** hacker) { mHacker = hacker; }

    void setCalcGroundNormalOrUpDir(bool isCalcGroundNormalOrUpDir) {
        mIsCalcGroundNormalOrUpDir = isCalcGroundNormalOrUpDir;
    }

private:
    al::LiveActor* mPlayer = nullptr;
    const PlayerConst* mConst = nullptr;
    const PlayerInput* mInput = nullptr;
    const IUsePlayerCollision* mCollision = nullptr;
    IUsePlayerHack** mHacker = nullptr;
    PlayerActionTurnControl* mTurnControl = nullptr;
    bool mIsSlerpGravity = false;
    bool mIsHoldJumpExtend = false;
    bool mIsForceHoldJumpExtend = false;
    bool mIsSnap2D = false;
    s32 mExtendFrame = 0;
    s32 mExtendFrameCounter = 0;
    s32 mNoInputFrame = 0;
    bool mIsCalcGroundNormalOrUpDir = false;
    bool mIsInertiaWall = false;
    bool mHasSpeedRange = false;
    u8 _43 = 0;
    f32 mSpeedMin = 0.0f;
    f32 mSpeedMax = 0.0f;
    sead::Vector3f mMoveDir = sead::Vector3f::zero;
    sead::Vector3f mSideDir = sead::Vector3f::zero;
    f32 mVelocityLimit = 0.0f;
    f32 mGravity = 0.0f;
    f32 mFallSpeedMax = 0.0f;
    bool mIsScaleVelocityInertiaWallHit = false;
    u8 _71 = 0;
    u8 _72 = 0;
    u8 _73 = 0;
    f32 mCollideWallScaleH = 1.0f;
    f32 mCollideWallScaleV = 0.0f;
    f32 mCollideWallScaleLimit = 0.0f;
    f32 mSlerpQuatRate = 0.0f;
    sead::Vector3f mTurnDir = sead::Vector3f::zero;
};

static_assert(sizeof(PlayerActionAirMoveControl) == 0x90);
