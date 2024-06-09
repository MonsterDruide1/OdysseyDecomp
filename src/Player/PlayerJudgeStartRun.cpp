#include "Player/PlayerJudgeStartRun.h"
#include "Player/PlayerCounterForceRun.h"
#include "Player/PlayerInput.h"
#include "Util/PlayerCollisionUtil.h"
#include "Util/StageSceneFunction.h"

PlayerJudgeStartRun::PlayerJudgeStartRun(const al::LiveActor* player, const PlayerConst* pConst,
                                         const IUsePlayerCollision* collision,
                                         const PlayerInput* input,
                                         const PlayerCounterForceRun* counterForceRun,
                                         const IJudge* judgeWaterSurfaceRun)
    : mPlayer(player), mCollision(collision), mInput(input), mCounterForceRun(counterForceRun),
      mJudgeForceLand(judgeWaterSurfaceRun) {}

// NON_MATCHING: Major mismatches for the control flow order
bool PlayerJudgeStartRun::judge() const {
    if (!rs::isCollidedGround(mCollision))
        return false;
    if (mCounterForceRun->isForceRun())
        return true;
    if (rs::isJudge(mJudgeForceLand))
        return false;
    if (mInput->isMove())
        return true;
    return rs::isAutoRunOnGroundSkateCode(mPlayer, mCollision, 0.5f);
}

void PlayerJudgeStartRun::reset() {}

void PlayerJudgeStartRun::update() {}
