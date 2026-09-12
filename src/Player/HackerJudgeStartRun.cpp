#include "Player/HackerJudgeStartRun.h"

#include "Library/LiveActor/ActorCollisionFunction.h"

#include "Player/PlayerCounterForceRun.h"
#include "Util/PlayerCollisionUtil.h"
#include "Util/PlayerHackInputFunction.h"

HackerJudgeStartRun::HackerJudgeStartRun(const al::LiveActor* parent, IUsePlayerHack** hacker)
    : HackerJudge(hacker), mParent(parent) {}

bool HackerJudgeStartRun::judge() const {
    if (!(mPlayerCollision ? rs::isCollidedGround(mPlayerCollision) :
                             al::isCollidedGround(mParent)))
        return false;
    if (rs::isOnHackMoveStick(*getHacker()))
        return true;
    if (mCounterForceRun && mCounterForceRun->isForceRun())
        return true;
    if (mPlayerCollision && mAutoRunSpeedThreshold > 0.0f &&
        rs::isAutoRunOnGroundSkateCode(mParent, mPlayerCollision, mAutoRunSpeedThreshold))
        return true;
    return false;
}
