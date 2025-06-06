#include "Player/PlayerJudgeStartWaterSurfaceRun.h"

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nature/WaterSurfaceFinder.h"

#include "Player/PlayerCounterForceRun.h"

PlayerJudgeStartWaterSurfaceRun::PlayerJudgeStartWaterSurfaceRun(
    const al::LiveActor* player, const al::WaterSurfaceFinder* waterSurfaceFinder,
    const PlayerCounterForceRun* counterForceRun)
    : mPlayer(player), mWaterSurfaceFinder(waterSurfaceFinder), mCounterForceRun(counterForceRun) {}

bool PlayerJudgeStartWaterSurfaceRun::judge() const {
    return mCounterForceRun->getCounter() > 0 && mWaterSurfaceFinder->isFoundSurface() &&
           al::isNearZeroOrGreater(mWaterSurfaceFinder->getDistance()) &&
           al::getGravity(mPlayer).dot(al::getVelocity(mPlayer)) > 0.0f;
}
