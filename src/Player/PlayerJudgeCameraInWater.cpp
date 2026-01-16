#include "Player/PlayerJudgeCameraInWater.h"

#include "Util/JudgeUtil.h"
#include "Util/PlayerUtil.h"

PlayerJudgeCameraInWater::PlayerJudgeCameraInWater(const al::LiveActor* player,
                                                   const IJudge* inWaterJudge,
                                                   const al::WaterSurfaceFinder* waterSurfaceFinder)
    : mPlayer(player), mInWaterJudge(inWaterJudge), mWaterSurfaceFinder(waterSurfaceFinder) {}

bool PlayerJudgeCameraInWater::judge() const {
    if (rs::isPlayerHack(mPlayer))
        return rs::isJudge(mInWaterJudge) && !mWaterSurfaceFinder->isFoundSurface();
    return rs::isJudge(mInWaterJudge) && !mWaterSurfaceFinder->isFoundSurface();
}
