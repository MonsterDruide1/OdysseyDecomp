#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
class AreaObj;
}  // namespace al
class PlayerModelHolder;

class PlayerAreaChecker {
public:
    PlayerAreaChecker(const al::LiveActor* player, const PlayerModelHolder* modelHolder);
    bool isInWater(const sead::Vector3f& pos) const;
    bool isInWaterDouble(const sead::Vector3f& pos, f32 gravityFactor) const;
    bool isInWaterWallCatch(const sead::Vector3f& pos, f32 gravityFactor) const;
    bool isInWaterIce(const sead::Vector3f& pos) const;
    bool isInWaterIceDouble(const sead::Vector3f& pos, f32 gravityFactor) const;
    bool isInWet(const sead::Vector3f& pos) const;
    bool isInRise(const sead::Vector3f& pos) const;
    bool isInHackCancel(const sead::Vector3f& pos) const;
    bool isInRecovery(const al::AreaObj** area, const sead::Vector3f& pos) const;
    bool isInRecoveryBan(const sead::Vector3f& pos) const;
    bool isInWallClimbBan(const sead::Vector3f& pos) const;
    bool isInForceRecovery(sead::Vector3f* targetPos, sead::Vector3f* targetUp,
                           const al::AreaObj** area, const sead::Vector3f& pos) const;
    bool isInShadowLength(f32* shadowLength, const sead::Vector3f& pos) const;
    bool isInCarryBan(const sead::Vector3f& pos) const;
    const al::AreaObj* tryFindStainArea(const sead::Vector3f& pos) const;
    const al::AreaObj* tryFindInvalidateInputFall(const sead::Vector3f& pos) const;

private:
    const al::LiveActor* mPlayer;
    const PlayerModelHolder* mModelHolder;
};

static_assert(sizeof(PlayerAreaChecker) == 0x10);
