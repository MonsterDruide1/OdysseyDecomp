#include "Player/PlayerJudgeStartRun.h"

#include "Player/PlayerCounterForceRun.h"
#include "Player/PlayerInput.h"
#include "Util/JudgeUtil.h"
#include "Util/PlayerCollisionUtil.h"

PlayerJudgeStartRun::PlayerJudgeStartRun(const al::LiveActor* player, const PlayerConst* pConst,
                                         const IUsePlayerCollision* collision,
                                         const PlayerInput* input,
                                         const PlayerCounterForceRun* counterForceRun,
                                         const IJudge* judgeWaterSurfaceRun)
    : mPlayer(player), mCollision(collision), mInput(input), mCounterForceRun(counterForceRun),
      mJudgeForceLand(judgeWaterSurfaceRun) {}

bool PlayerJudgeStartRun::judge() const {
    if (!rs::isCollidedGround(mCollision))
        return false;
    if (mCounterForceRun->isForceRun())
        return true;
    if (!rs::isJudge(mJudgeForceLand) &&
        (mInput->isMove() || rs::isAutoRunOnGroundSkateCode(mPlayer, mCollision, 0.5f)))
        return true;
    return false;
}

void PlayerJudgeStartRun::reset() {}

void PlayerJudgeStartRun::update() {}
