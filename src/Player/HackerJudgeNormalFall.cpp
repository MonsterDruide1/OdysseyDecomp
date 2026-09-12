#include "Player/HackerJudgeNormalFall.h"

#include <math/seadMathCalcCommon.h>

#include "Library/LiveActor/ActorCollisionFunction.h"

#include "Util/PlayerCollisionUtil.h"

HackerJudgeNormalFall::HackerJudgeNormalFall(const al::LiveActor* parent, s32 fallFrame)
    : mActor(parent), mFallFrame(fallFrame) {}

void HackerJudgeNormalFall::reset() {
    mFramesNoCollideGround = 0;
}

// The original materializes the counter address separately in each collision branch.
void HackerJudgeNormalFall::update() {
    if (mPlayerCollision)
        mFramesNoCollideGround = rs::isCollidedGround(mPlayerCollision) ?
                                     0 :
                                     sead::Mathi::clampMax(mFramesNoCollideGround + 1, mFallFrame);
    else
        mFramesNoCollideGround = al::isCollidedGround(mActor) ?
                                     0 :
                                     sead::Mathi::clampMax(mFramesNoCollideGround + 1, mFallFrame);
}

bool HackerJudgeNormalFall::judge() const {
    return mFramesNoCollideGround >= mFallFrame;
}
