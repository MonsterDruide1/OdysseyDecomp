#include "Player/PlayerAreaChecker.h"

#include "Library/Area/AreaObjUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nature/NatureUtil.h"

#include "System/GameDataUtil.h"
#include "Util/AreaUtil.h"
#include "Util/ObjUtil.h"

PlayerAreaChecker::PlayerAreaChecker(const al::LiveActor* player,
                                     const PlayerModelHolder* modelHolder)
    : mPlayer(player), mModelHolder(modelHolder) {}

bool PlayerAreaChecker::isInWater(const sead::Vector3f& pos) const {
    return al::isInWaterPos(mPlayer, pos);
}

bool PlayerAreaChecker::isInWaterDouble(const sead::Vector3f& pos, f32 gravityFactor) const {
    return isInWater(pos) || isInWater(pos - al::getGravity(mPlayer) * gravityFactor);
}

bool PlayerAreaChecker::isInWaterWallCatch(const sead::Vector3f& pos, f32 gravityFactor) const {
    sead::Vector3f rootOffset = {0.0f, 0.0f, 0.0f};
    rs::calcOffsetAllRoot(&rootOffset, mModelHolder);
    return isInWater(pos + rootOffset - al::getGravity(mPlayer) * gravityFactor);
}

bool PlayerAreaChecker::isInWaterIce(const sead::Vector3f& pos) const {
    return al::isInIceWaterPos(mPlayer, pos);
}

bool PlayerAreaChecker::isInWaterIceDouble(const sead::Vector3f& pos, f32 gravityFactor) const {
    // second one must be written like this to match
    return isInWaterIce(pos) ||
           al::isInIceWaterPos(mPlayer, pos - al::getGravity(mPlayer) * gravityFactor);
}

bool PlayerAreaChecker::isInWet(const sead::Vector3f& pos) const {
    return al::isInAreaObj(mPlayer, "WetArea", pos);
}

bool PlayerAreaChecker::isInRise(const sead::Vector3f& pos) const {
    return al::isInAreaObj(mPlayer, "RiseArea", pos);
}

bool PlayerAreaChecker::isInHackCancel(const sead::Vector3f& pos) const {
    return al::isInAreaObj(mPlayer, "HackCancelArea", pos);
}

bool PlayerAreaChecker::isInRecovery(const al::AreaObj** area, const sead::Vector3f& pos) const {
    const al::AreaObj* recoveryArea = al::tryFindAreaObj(mPlayer, "RecoveryArea", pos);
    if (recoveryArea) {
        *area = recoveryArea;
        return true;
    }
    return false;
}

bool PlayerAreaChecker::isInRecoveryBan(const sead::Vector3f& pos) const {
    return al::isInAreaObj(mPlayer, "RecoveryBanArea", pos);
}

bool PlayerAreaChecker::isInWallClimbBan(const sead::Vector3f& pos) const {
    return al::isInAreaObj(mPlayer, "InvalidateWallClimbArea", pos);
}

bool PlayerAreaChecker::isInForceRecovery(sead::Vector3f* targetPos, sead::Vector3f* targetUp,
                                          const al::AreaObj** area,
                                          const sead::Vector3f& pos) const {
    return rs::isKidsMode(mPlayer) &&
           rs::tryFindForceRecoveryArea(targetPos, targetUp, area, mPlayer, pos);
}

bool PlayerAreaChecker::isInShadowLength(f32* shadowLength, const sead::Vector3f& pos) const {
    const al::AreaObj* shadowArea = al::tryFindAreaObj(mPlayer, "PlayerShadowLengthArea", pos);
    if (shadowArea)
        return al::tryGetAreaObjArg(shadowLength, shadowArea, "ShadowLength");
    return false;
}

bool PlayerAreaChecker::isInCarryBan(const sead::Vector3f& pos) const {
    return al::isInAreaObj(mPlayer, "CarryBanArea", pos);
}

const al::AreaObj* PlayerAreaChecker::tryFindStainArea(const sead::Vector3f& pos) const {
    return al::tryFindAreaObj(mPlayer, "StainArea", pos);
}

const al::AreaObj* PlayerAreaChecker::tryFindInvalidateInputFall(const sead::Vector3f& pos) const {
    return al::tryFindAreaObj(mPlayer, "InvalidateInputFallArea", pos);
}
