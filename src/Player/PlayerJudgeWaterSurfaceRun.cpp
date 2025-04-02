#include "Player/PlayerJudgeWaterSurfaceRun.h"

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nature/WaterSurfaceFinder.h"

#include "Player/PlayerConst.h"
#include "Player/PlayerCounterForceRun.h"

PlayerJudgeWaterSurfaceRun::PlayerJudgeWaterSurfaceRun(
    const al::LiveActor* player, const PlayerConst* pConst,
    const al::WaterSurfaceFinder* waterSurfaceFinder, const PlayerCounterForceRun* counterForceRun)
    : mPlayer(player), mConst(pConst), mWaterSurfaceFinder(waterSurfaceFinder),
      mCounterForceRun(counterForceRun) {}

f32 PlayerJudgeWaterSurfaceRun::getBorderSpeedH() const {
    return mConst->getDashFastBorderSpeed();
}

void PlayerJudgeWaterSurfaceRun::update() {
    mIsWaterSurfaceRun = false;
    if (!mIsEnable)
        return;

    const al::LiveActor* player = mPlayer;
    f32 borderSpeedH = getBorderSpeedH();
    const PlayerCounterForceRun* counterForceRun = mCounterForceRun;
    mIsWaterSurfaceRun =
        mWaterSurfaceFinder->isFoundSurface() &&
        al::isNearZeroOrGreater(mWaterSurfaceFinder->getDistance(), 0.001f) &&
        (counterForceRun->getCounter() > 0 || al::calcSpeedH(player) > borderSpeedH);
}

void PlayerJudgeWaterSurfaceRun::reset() {
    mIsWaterSurfaceRun = false;
}

bool PlayerJudgeWaterSurfaceRun::judge() const {
    return mIsWaterSurfaceRun;
}
