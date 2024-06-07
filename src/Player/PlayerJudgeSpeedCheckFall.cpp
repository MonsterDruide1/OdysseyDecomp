#include "Player/PlayerJudgeSpeedCheckFall.h"

#include <math/seadMathCalcCommon.h>

#include "Util/PlayerCollisionUtil.h"
#include "Util/StageSceneFunction.h"

PlayerJudgeSpeedCheckFall::PlayerJudgeSpeedCheckFall(const al::LiveActor* player,
                                                     const IUsePlayerCollision* collision,
                                                     const PlayerConst* pConst,
                                                     const IJudge* judgeWaterSurfaceRun)
    : mPlayer(player), mCollision(collision), mConst(pConst),
      mJudgeStartWaterSurfaceRun(judgeWaterSurfaceRun) {}

void PlayerJudgeSpeedCheckFall::reset() {
    mFramesNoCollideGround = 0;
}

void PlayerJudgeSpeedCheckFall::update() {
    if (!rs::isCollidedGroundRunAngle(mPlayer, mCollision, mConst)) {
        if (mFramesNoCollideGround <= 4)
            mFramesNoCollideGround++;
    } else {
        mFramesNoCollideGround = 0;
    }
}

bool PlayerJudgeSpeedCheckFall::judge() const {
    return !rs::isJudge(mJudgeStartWaterSurfaceRun) && mFramesNoCollideGround > 4;
}
