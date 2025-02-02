#include "Player/PlayerJudgeDiveInWater.h"

#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nature/NatureUtil.h"

#include "Player/IUsePlayerFallDistanceCheck.h"
#include "Player/IUsePlayerHeightCheck.h"
#include "Player/PlayerConst.h"

PlayerJudgeDiveInWater::PlayerJudgeDiveInWater(const al::LiveActor* player,
                                               const PlayerConst* playerConst,
                                               const IUsePlayerHeightCheck* heightCheck,
                                               const IUsePlayerFallDistanceCheck* fallDistanceCheck)
    : mPlayer(player), mConst(playerConst), mHeightCheck(heightCheck),
      mFallDistanceCheck(fallDistanceCheck) {}

bool PlayerJudgeDiveInWater::judge() const {
    if (mFallDistanceCheck->getFallDistance() < 10.0f)
        return false;
    if (mHeightCheck->isAboveGround() && mHeightCheck->getGroundHeight() < 1500.0f)
        return false;

    sead::Vector3f up = -al::getGravity(mPlayer);
    f32 findDistance = (mConst->getTall() + 1500.0f) * 0.5f;
    sead::Vector3f findStart = al::getTrans(mPlayer) + -up * findDistance;

    sead::Vector3f surfacePos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f surfaceNormal = {0.0f, 0.0f, 0.0f};
    if (!al::calcFindWaterSurfaceFlat(&surfacePos, &surfaceNormal, mPlayer, findStart, up,
                                      findDistance))
        return false;

    f32 surfaceDistance = (al::getTrans(mPlayer) - surfacePos).dot(up);
    if (surfaceDistance < 1500.0f)
        return false;

    if (mHeightCheck->isAboveGround()) {
        f32 minHeight = (surfaceDistance + mConst->getTall());
        if (mHeightCheck->getGroundHeight() < minHeight)
            return false;
    }

    return true;
}

void PlayerJudgeDiveInWater::reset() {}

void PlayerJudgeDiveInWater::update() {}
